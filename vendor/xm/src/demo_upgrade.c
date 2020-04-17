
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_upgrade.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示系统升级相关功能。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#include "demo_inc.h"
#include "demo_upgrade.h"
#include "demo_upgrade_sparse.h"


#define MTD_PROC	"/proc/mtd"

#define MTD_START	0x00000000
#define MTD_RDWR_BLOCK_MIN	0x10000


typedef struct
{
	XM_S32 bEnabled;
	XM_CHAR aMtdPart[32];
	XM_CHAR aUpgradeFile[128];
} UpgradeModParam_s;

typedef struct
{
	XM_S32 index;
	XM_U32 startAddr;
	XM_U32 size;
} UpgradeMtdInfo_s;

static XM_S32 Demo_Upgrade_getRdWrBlockSize(XM_VOID)
{
	XM_S32 blockSize;
	
	blockSize = LibXmMaQue_Flash_getEraseSize();

	if (blockSize < MTD_RDWR_BLOCK_MIN)
	{
		blockSize = MTD_RDWR_BLOCK_MIN;
	}

	return blockSize;
}

static XM_VOID Demo_Upgrade_ModParam_print(XM_CHAR *pOwner, UpgradeModParam_s *pstModParam)
{
	SIMPLE_PRINTF("@%s->Demo_Upgrade_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstModParam->bEnabled);
	SIMPLE_PRINTF("\taMtdPart: %s\n", pstModParam->aMtdPart);
	SIMPLE_PRINTF("\taUpgradeFile: %s\n", pstModParam->aUpgradeFile);
}

static XM_S32 Demo_Upgrade_getMtdIndex(XM_CHAR *pPartName, UpgradeMtdInfo_s *pstMtdInfo)
{
	FILE *pFile;
	XM_CHAR aLine[256];
	XM_CHAR aName[32];
	XM_S32 index, size, eraseSize;
	XM_S32 bFound = 0;
	XM_U32 startAddr;
	
	if ((pFile = fopen(MTD_PROC, "r")) == NULL)
	{
		ERROR_PRINTF("fopen Failed:%s.\n",MTD_PROC);
		return XM_FAILURE;
	}
	
	if (!fgets(aLine, sizeof(aLine), pFile))
	{
		fclose(pFile);
		ERROR_PRINTF("fgets Failed:%s.\n", MTD_PROC);
		return XM_FAILURE;
	}
	
	if(strstr(aLine, "dev:") == NULL)
	{
		fclose(pFile);
		
		ERROR_PRINTF("strstr 'dev:' Failed:%s.\n", MTD_PROC);
		
		return XM_FAILURE;
	}

	startAddr = MTD_START;
	
	while(fgets(aLine, sizeof(aLine), pFile))
	{
		if(strstr(aLine, "mtd") != NULL)
		{
			memset(aName, 0, sizeof(aName));
			
			sscanf(aLine,"mtd%d: %x %x \"%[^\"]\"", &index, &size, &eraseSize, aName);

			INFO_PRINTF("index: %d, size: 0x%x, size: 0x%x, aName: %s, pPartName: %s.\n", 
				index, size, eraseSize, aName, pPartName);

			if (!strcasecmp(aName, pPartName))
			{
				pstMtdInfo->index = index;
				pstMtdInfo->startAddr = startAddr;
				pstMtdInfo->size = size;
				bFound = 1;
				break;
			}

			startAddr += size;
		}
		else
		{
			break;
		}
	}
	
	fclose(pFile);	
	
	return bFound ? XM_SUCCESS : XM_FAILURE;
}

static XM_S32 Demo_Upgrade_onePart(XM_CHAR *pPart)
{
	XM_S32 res;
	XM_S32 fileSize;
	XM_S32 blockSize, readSize, totalWriteSize = 0;
	XM_S32 curPercent, lastPercent = -1;
	XM_U32 flashStartAddr;
	XM_U8 *pBuf = NULL;
	XM_U8 *pRawFlashBuffer = NULL;
	XM_S32 fd = -1;
	UpgradeMtdInfo_s stMtdInfo;
	UpgradeModParam_s stModParam = { 0 };
	StringValueGet_s astUpgradeValGet[] = 
	{
		{"bEnabled:", &stModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stModParam.bEnabled)},
		{"MtdPart:", &stModParam.aMtdPart, XM_VALUE_TYPE_STRING, sizeof(stModParam.aMtdPart)},
		{"UpgradeFile:", &stModParam.aUpgradeFile, XM_VALUE_TYPE_STRING, sizeof(stModParam.aUpgradeFile)},	
	};
	XM_CHAR *pEnv = NULL;
	XM_CHAR *pUpgradeFile = NULL;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			pPart, astUpgradeValGet, sizeof(astUpgradeValGet) / sizeof(StringValueGet_s));

	if (!stModParam.bEnabled)
	{
		INFO_PRINTF("Upgrade %s Not Enabled!\n", pPart);	
		return XM_SUCCESS;
	}

	Demo_Upgrade_ModParam_print(pPart, &stModParam);

	res = Demo_Upgrade_getMtdIndex(stModParam.aMtdPart, &stMtdInfo);
	if (res != XM_SUCCESS)
	{
		INFO_PRINTF("Demo_Upgrade_getMtdIndex Failed, part: %s!\n", stModParam.aMtdPart);	
		return XM_FAILURE;
	}

	INFO_PRINTF("Part: %s, index: %d, size: %d, startAddr: 0x%x !\n", 
		pPart, stMtdInfo.index, stMtdInfo.size, stMtdInfo.startAddr);

	LibXmMaQue_Watchdog_enable(0);

	LibXmMaQue_Flash_open();

	blockSize = Demo_Upgrade_getRdWrBlockSize();

	INFO_PRINTF("blockSize: %d.\n", blockSize);

	pBuf =  (XM_U8 *)malloc(blockSize);
	if (!pBuf)
	{
		ERROR_PRINTF("malloc pBuf Failed: %d.\n", blockSize);
		
		res = XM_FAILURE;

		goto ExitHandler;
	}

	pRawFlashBuffer =  (XM_U8 *)malloc(blockSize);
	if (!pRawFlashBuffer)
	{
		ERROR_PRINTF("malloc pRawFlashBuffer Failed: %d.\n", blockSize);
		
		res = XM_FAILURE;

		goto ExitHandler;
	}

	pUpgradeFile = stModParam.aUpgradeFile;

	pEnv = getenv("MAQUE_UPGRADE_FILE");
	if (pEnv)
	{
		INFO_PRINTF("pEnv: %s.\n", pEnv);
		pUpgradeFile = pEnv;
	}

	fileSize = MaQue_Demo_File_getSize(pUpgradeFile);
	if (fileSize <= 0)
	{
		ERROR_PRINTF("Invalid FileSize: %d.\n", fileSize);
		
		res = XM_FAILURE;

		goto ExitHandler;
	}
	
	if ((fd = open(pUpgradeFile, O_RDONLY)) < 0)
	{
		ERROR_PRINTF("open() Failed:%s\n", pUpgradeFile);
		
		res = XM_FAILURE;

		goto ExitHandler;
	}

	INFO_PRINTF("---------------- Upgrading %s Start ----------------\n", pPart);
	flashStartAddr = stMtdInfo.startAddr;
		
	while((readSize = MaQue_Demo_File_read(fd, pBuf, blockSize)) > 0)
	{
		if (LibXmMaQue_Flash_read(pRawFlashBuffer, flashStartAddr, blockSize) < 0)
		{
			res = XM_FAILURE;
			goto ExitHandler;
		}

		usleep(1000);
		// 比较数据，提高速度
		if (memcmp(pRawFlashBuffer, pBuf, readSize))
		{
			memcpy(pRawFlashBuffer, pBuf, readSize);   //注意数据交换，不要随便修改，也不要漏掉这个操作
			usleep(1000);
			res = LibXmMaQue_Flash_write(pRawFlashBuffer, flashStartAddr, blockSize);
			if (res <= 0)
			{
				ERROR_PRINTF("LibXmMaQue_Flash_write() Failed, UpgradeFile: %s, StartAddr: 0x%x, WriteSize: %d\n", 
					pUpgradeFile, flashStartAddr, blockSize);
				res = XM_FAILURE;
				goto ExitHandler;
			}
		}


		flashStartAddr += readSize;

		totalWriteSize += readSize;
		curPercent = (totalWriteSize  * 100) / fileSize;

		if (lastPercent != curPercent)
		{
			INFO_PRINTF("Upgrade Percent: %d\n", curPercent); 
			lastPercent = curPercent;
		}
	}

	INFO_PRINTF("\n---------------- Upgrading %s End ----------------\n", pPart);

	res = XM_SUCCESS;
		
ExitHandler:

	if (pBuf)
	{
		free(pBuf);
	}

	if (pRawFlashBuffer)
	{
		free(pRawFlashBuffer);
	}

	if (fd > 0)
	{
		close(fd);
	}

	LibXmMaQue_Flash_close();
	
	LibXmMaQue_Watchdog_enable(1);
	
	return res;
}


static XM_VOID *Demo_Upgrade_task(XM_VOID *pArg)
{
	Demo_Upgrade_onePart("UpgradeRomfs");
	Demo_Upgrade_onePart("UpgradeUser");

	return NULL;
}

XM_S32 MaQue_Demo_Upgrade_start(XM_VOID)
{
	XM_S32 bRomfsEnabled = 0, bEnabled = 0;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"UpgradeRomfs", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			bRomfsEnabled = 1;	
		}
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"UpgradeUser", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));
	}

	if (bRomfsEnabled || bEnabled)
	{
		LibXmMaQue_System_prepareForUpgrade();
		MaQue_Demo_Common_createThread(Demo_Upgrade_task, NULL, "Upgrage");
	}
	
	return XM_SUCCESS;
}

/********************** XmMaQueSdkDemo -u <Image File>, Start. *************************/

#define FLASH_SIZE_MAX	0x2000000

#define IH_MAGIC	0x27051956	/* Image Magic Number		*/

typedef struct
{
    XM_U32 ih_magic;   /* Image Header Magic Number    */
    XM_U32 ih_hcrc;    /* Image Header CRC Checksum    */
    XM_U32 ih_time;    /* Image Creation Timestamp */
    XM_U32 ih_size;    /* Image Data Size      */
    XM_U32 ih_load;    /* Data  Load  Address      */
    XM_U32 ih_ep;      /* Entry Point Address      */
    XM_U32 ih_dcrc;    /* Image Data CRC Checksum  */
    XM_U8 ih_os;      /* Operating System     */
    XM_U8 ih_arch;    /* CPU architecture     */
    XM_U8 ih_type;    /* Image Type           */
    XM_U8 ih_comp;    /* Compression Type     */
    XM_U8 ih_name[32];/* Image Name       */
} ImageHeader_s;


#define PACK_ID "XM-DVR"

typedef struct
{
	XM_U8 id[16];			/* product id, should be "XM-DVR" */
	XM_U32 crc;				/* crc code */
	XM_U32 head_len;		/* header length */
	XM_U32 total_len;		/* packet length */
	XM_U32 pkt_type;		/* packet type */
	XM_U8 vendor[16];		/* vendor name */
	XM_U8 reserved[16];		/* reserved struct */
} PktHeader_s;

//#define UPGRADE_BUF_COUNT_MAX	512

typedef struct
{
	XM_U32 actSize;
	XM_U8 *pBuf;
} UpgradeBuf_s;

typedef struct
{
	XM_S32 count;
	UpgradeBuf_s *pstBuf;
} UpgradeBufMgr_s;


static XM_VOID Demo_Upgrade_BufMgr_free(UpgradeBufMgr_s *pstBufMgr)
{
	XM_S32 i;

	if (pstBufMgr->count <= 0 || !pstBufMgr->pstBuf)
	{
		return;
	}
	
	for (i = 0; i < pstBufMgr->count; i++)
	{
		if (!pstBufMgr->pstBuf[i].pBuf)
		{
			continue;
		}

		free(pstBufMgr->pstBuf[i].pBuf);
		pstBufMgr->pstBuf[i].pBuf = NULL;
	}

	free(pstBufMgr->pstBuf);
	pstBufMgr->pstBuf = NULL;
	pstBufMgr->count = 0;
}

static XM_VOID Demo_Upgrade_printImgHeader(ImageHeader_s *pstImgHdr)
{
	SIMPLE_PRINTF("Image Name: %s\n", pstImgHdr->ih_name);
	SIMPLE_PRINTF("Image Type: %d\n", pstImgHdr->ih_type);
	SIMPLE_PRINTF("Data Size: %d\n", ntohl(pstImgHdr->ih_size));

	SIMPLE_PRINTF("Load Address: %08x\n", ntohl(pstImgHdr->ih_load));
	SIMPLE_PRINTF("Entry Point: %08x\n", ntohl(pstImgHdr->ih_ep));
}

static XM_S32 Demo_Upgrade_checkImgHeader(ImageHeader_s *pstImgHeader)
{
	XM_S32 len;
	XM_U32 checksum, crcRes;

	if (ntohl(pstImgHeader->ih_magic) != IH_MAGIC)
	{
		ERROR_PRINTF("Bad Magic Number: 0x%x.\n", ntohl(pstImgHeader->ih_magic));
		return XM_FAILURE;
	}

	len = sizeof(ImageHeader_s);
	checksum = ntohl(pstImgHeader->ih_hcrc);
	pstImgHeader->ih_hcrc = 0;

	if ((crcRes = LibXmMaQue_Crc_crc32(0, (const XM_U8 *)pstImgHeader, len)) != checksum)
	{
		ERROR_PRINTF("Header CRC Checking Failed: 0x%x != org: 0x%x.\n", crcRes, checksum);
		return XM_FAILURE;
	}
	else
	{
		INFO_PRINTF("Header CRC Checking OK\n");
	}
	
	Demo_Upgrade_printImgHeader(pstImgHeader);

	return XM_SUCCESS;
}

XM_S32 Demo_Upgrade_updateOneBlock(XM_UL flashAddr, XM_U8 *pData, XM_S32 len)
{
	XM_S32 res, blockSize;
	XM_U8 *pBuf;
	
	if (len <= 0)
	{
		ERROR_PRINTF("Invalid len: %d.\n", len);
		return XM_SUCCESS;
	}

	blockSize = Demo_Upgrade_getRdWrBlockSize();
	
	pBuf = (XM_U8 *)malloc(blockSize);
	if (!pBuf)
	{
		ERROR_PRINTF("malloc pBuf Failed: %d.\n", blockSize);
		return XM_FAILURE;
	}

	res = LibXmMaQue_Flash_read(pBuf, flashAddr, blockSize);
	if(res != blockSize)
	{
		free(pBuf);
		ERROR_PRINTF("LibXmMaQue_Flash_read Failed, res: %d, != len: %d.\n", res, blockSize);
		return XM_FAILURE;
	}

	if (!memcmp(pData, pBuf, len))
	{
		free(pBuf);
		INFO_PRINTF ("No need to upgrade the flash addr: 0x%lX.\n", flashAddr);
		return XM_SUCCESS;
	}

	if (len != blockSize)
	{
		memcpy(&pData[len], &pBuf[len], blockSize - len);
	}
		
	res = LibXmMaQue_Flash_write(pData, flashAddr, blockSize);
	if(res != blockSize)
	{
		ERROR_PRINTF("LibXmMaQue_Flash_write() Failed, flash addr: 0x%lX!\n", flashAddr);
		res = XM_FAILURE;
	}
	else
	{
		INFO_PRINTF("LibXmMaQue_Flash_write() OK, flash addr: 0x%lX!\n", flashAddr);
		res = XM_SUCCESS;
	}

	free(pBuf);

	return res;
}

static XM_S32 Demo_Upgrade_updateOnePart (XM_CHAR *pUpgradeFile, XM_U32 offset, 
			ImageHeader_s *pstImgHdr, UpgradeBufMgr_s *pstBufMgr, XM_U32 nBlocks)
{
	XM_S32 res = XM_SUCCESS, i;
	XM_U32 len;
	XM_U32 crcRes = 0;
	XM_CHAR *pType = NULL;
	XM_U64 flashAddr, flashAddrStart, flashAddrEnd;
	XM_S32 curPercent, lastPercent = -1;

	INFO_PRINTF("Start Upgrading Image Name: %s, nBlocks: %d.\n", pstImgHdr->ih_name, nBlocks);

	flashAddrStart = ntohl(pstImgHdr->ih_load);	/* Flash writing address */
	flashAddrEnd = ntohl(pstImgHdr->ih_ep);

	INFO_PRINTF("Programing start at: 0x%08llx\n", flashAddrStart);
	INFO_PRINTF("Programing end at: 0x%08llx\n", flashAddrEnd);

	if (flashAddrStart < 0x0)
	{
		ERROR_PRINTF("Invalid DestAddr, must be >= 0x00000000.\n");
		return XM_FAILURE;
	}

	len = ntohl(pstImgHdr->ih_size);

	INFO_PRINTF("Programing len: 0x%08X\n", len);

	for (i = 0; i < nBlocks; i++)
	{
		crcRes = LibXmMaQue_Crc_crc32(crcRes, pstBufMgr->pstBuf[i].pBuf, pstBufMgr->pstBuf[i].actSize);
	}

	if (crcRes != ntohl(pstImgHdr->ih_dcrc))
	{
		ERROR_PRINTF("Data CRC Checking Failed: 0x%X != org: 0x%X.\n", crcRes, ntohl(pstImgHdr->ih_dcrc));
		return XM_FAILURE;
	}
	else
	{
		INFO_PRINTF ("Data CRC Checking OK\n");
	}

	{
		XM_CHAR *pDat = (XM_CHAR *)pstBufMgr->pstBuf[0].pBuf;

		if(pDat[0] == 'Y' && pDat[1] == 'F' && pDat[2] == 'S' && pDat[3] == 'S')
		{
			pType = "yaffs2";
			ERROR_PRINTF ("Not Support pType: %s.\n", pType);
			return XM_FAILURE;
		}
	}

	{
		XM_CHAR *pDat = (XM_CHAR *)pstBufMgr->pstBuf[0].pBuf;
		
		if (0x3A == pDat[0] && 0xFF == pDat[1] && 0x26 == pDat[2] && 0xED == pDat[3])
		{
			pType = "ext4sp";
			
			INFO_PRINTF("pType: %s.\n", pType);
			
			return Demo_Upgrade_Sparce_writeExt4Sp(pUpgradeFile, offset, flashAddrStart);
		}
	}
#if 0	
	if(Demo_Upgrade_isUpgraded(flashAddrStart, len, ntohl(pstImgHdr->ih_dcrc), pType))
	{
		INFO_PRINTF ("No need to upgrade the same version.\n");
		return 0;
	}
#endif

	flashAddr = flashAddrStart;
	
	for (i = 0; i < nBlocks; i++)
	{
		res |= Demo_Upgrade_updateOneBlock(flashAddr, pstBufMgr->pstBuf[i].pBuf, pstBufMgr->pstBuf[i].actSize);

		flashAddr += pstBufMgr->pstBuf[i].actSize;

		curPercent = (i * 100) / nBlocks;

		if (curPercent != lastPercent)
		{
			INFO_PRINTF("Upgrading Percent: %d.\n", curPercent);
			lastPercent = curPercent;
		}
	}

	if (XM_SUCCESS == res)
	{
		INFO_PRINTF("Upgrading OK, Image Name: %s.\n", pstImgHdr->ih_name);
	}
	else
	{
		INFO_PRINTF("Upgrading Failed, Image Name: %s.\n", pstImgHdr->ih_name);
	}	

	return res;
}

static XM_S32 Demo_Upgrade_upgrade (XM_CHAR *pUpgradeFile, XM_S32 fd, XM_U8 *pHdrBuf)
{
	XM_S32 ret, i = 0;
	PktHeader_s *pHdr = NULL;
	XM_S32 totalLen, headLen, imgLen;
	XM_S32 index = 1;
	XM_S32 bUpdateImg = 0;
	UpgradeBufMgr_s stBufMgr = { 0 };
	XM_S32 readSize, totalProcSize = 0;
	XM_S32 dataSize, nBlocks, blockSize;
	XM_S32 actRdSize, rmdSize;

	pHdr = (PktHeader_s*)pHdrBuf;

	ret = strncmp((const char *)pHdr->id, PACK_ID, strlen(PACK_ID));
	if (ret != 0)
	{
		INFO_PRINTF("Invalid PACK_ID, May be general image.\n");
		bUpdateImg = 0;
	}
	else
	{
		INFO_PRINTF("Valid PACK_ID.\n");
		bUpdateImg = 1;
	}	
	
	if (bUpdateImg)
	{
		INFO_PRINTF("PACK_ID OK\n");
		totalLen = pHdr->total_len;
		headLen = pHdr->head_len;
	}
	else
	{
		headLen = 0;
		totalLen = 0;  /* total_len设置为0，确保下面只处理一次。*/
	}

	ret = lseek(fd, headLen, SEEK_SET);
	if (ret < 0)
	{
		ERROR_PRINTF("lseek() Failed, head_len: %d, Error: %s.\n", headLen, strerror(errno));
		return XM_FAILURE;
	}

	totalProcSize += headLen;

	blockSize = Demo_Upgrade_getRdWrBlockSize();

	do
	{
		readSize = MaQue_Demo_File_read(fd, pHdrBuf, sizeof(ImageHeader_s));
		if (readSize < sizeof(ImageHeader_s))
		{
			ERROR_PRINTF("read() file header Failed, ReadSize: %d\n", readSize);
			ret = XM_FAILURE;
			goto ExitHandler;
		}

		if (Demo_Upgrade_checkImgHeader((ImageHeader_s *)pHdrBuf) != XM_SUCCESS)
		{
			ret = XM_FAILURE;
			goto ExitHandler;
		}
		
		dataSize = ntohl(((ImageHeader_s *)pHdrBuf)->ih_size);
		if (dataSize <= 0)
		{
			ret = XM_FAILURE;
			goto ExitHandler;
		}

		nBlocks = (dataSize + blockSize - 1) / blockSize;
		if (nBlocks <= 0)
		{
			ERROR_PRINTF("Invalid nBlocks: %d\n", nBlocks);
			ret = XM_FAILURE;
			goto ExitHandler;
		}

		stBufMgr.pstBuf = calloc(nBlocks, sizeof(UpgradeBuf_s));
		if (!stBufMgr.pstBuf)
		{
			ERROR_PRINTF("calloc stBufMgr.pstBuf Failed!\n");
			ret = XM_FAILURE;
			goto ExitHandler;
		}

		stBufMgr.count = nBlocks;

		for (i = 0; i < nBlocks; i++)
		{
			stBufMgr.pstBuf[i].pBuf = malloc(blockSize);
			if (!stBufMgr.pstBuf[i].pBuf)
			{
				ERROR_PRINTF("malloc stBufMgr.pstBuf[%d].pBuf Failed!\n", i);
				ret = XM_FAILURE;
				goto ExitHandler;
			}
		}

		imgLen = ntohl(((ImageHeader_s *)pHdrBuf)->ih_size) + sizeof(ImageHeader_s);

		rmdSize = dataSize;
		index = 0;

		do 
		{
			actRdSize = blockSize;
			
			if (blockSize > rmdSize)
			{
				actRdSize = rmdSize;
			}
			
			readSize = MaQue_Demo_File_read(fd, stBufMgr.pstBuf[index].pBuf, actRdSize);
			if (readSize != actRdSize)
			{
				ERROR_PRINTF("read() file Failed, ReadSize: %d != actRdSize: %d.\n", readSize, actRdSize);
				ret = XM_FAILURE;
				goto ExitHandler;
			}

			stBufMgr.pstBuf[index].actSize = actRdSize; 

			rmdSize -= readSize;

			index++;
		
		} while(rmdSize > 0);
	
		if (Demo_Upgrade_updateOnePart(pUpgradeFile, totalProcSize + sizeof(ImageHeader_s), 
				(ImageHeader_s *)pHdrBuf, &stBufMgr, index))
		{
			ERROR_PRINTF("Demo_Upgrade_updateOnePart Failed, nBlocks: %d!\n", index);
			ret = XM_FAILURE;
			goto ExitHandler;
		}

		Demo_Upgrade_BufMgr_free(&stBufMgr);

		totalProcSize += imgLen;
		
	} while(totalProcSize < totalLen);

	INFO_PRINTF("Upgrading is complete.\n");

	ret = XM_SUCCESS;

ExitHandler:

	Demo_Upgrade_BufMgr_free(&stBufMgr);

	return ret;
} /* end do_flwrite */

XM_S32 MaQue_Demo_Upgrade(XM_CHAR *pUpgradeFile)
{
	XM_S32 res;
	XM_S32 fileSize;
	XM_S32 blockSize, readSize;
	XM_U8 *pHdrBuf = NULL;
	XM_S32 fd = -1;
	XM_S32 headerLen;
	
	LibXmMaQue_System_prepareForUpgrade();

	LibXmMaQue_Watchdog_enable(0);

	LibXmMaQue_Flash_open();

	blockSize = LibXmMaQue_Flash_getEraseSize();

	INFO_PRINTF("blockSize: %d.\n", blockSize);

	if (blockSize != 0x10000 && strcasecmp(LibXmMaQue_Flash_getName(), "eMMC"))
	{
		ERROR_PRINTF("Invalid blockSize: %d, Flash Name:%s.\n", blockSize, LibXmMaQue_Flash_getName());
		res = XM_FAILURE;
		goto ExitHandler;
	}

	headerLen = sizeof(ImageHeader_s) > sizeof(PktHeader_s) ? sizeof(ImageHeader_s) : sizeof(PktHeader_s);

	pHdrBuf =  (XM_U8 *)malloc(headerLen);
	if (!pHdrBuf)
	{
		ERROR_PRINTF("malloc pHdrBuf Failed: %d.\n", headerLen);
		res = XM_FAILURE;
		goto ExitHandler;
	}

	fileSize = MaQue_Demo_File_getSize(pUpgradeFile);
	if (fileSize <= 0)
	{
		ERROR_PRINTF("Invalid FileSize: %d.\n", fileSize);
		res = XM_FAILURE;
		goto ExitHandler;
	}
	
	if ((fd = open(pUpgradeFile, O_RDONLY)) < 0)
	{
		ERROR_PRINTF("open() Failed:%s\n", pUpgradeFile);
		res = XM_FAILURE;
		goto ExitHandler;
	}

	readSize = MaQue_Demo_File_read(fd, pHdrBuf, headerLen);
	if (readSize < sizeof(PktHeader_s) && readSize < sizeof(ImageHeader_s))
	{
		ERROR_PRINTF("read() file header Failed, ReadSize: %d, File: %s\n", readSize, pUpgradeFile);
		
		res = XM_FAILURE;

		goto ExitHandler;
	}

	res = Demo_Upgrade_upgrade(pUpgradeFile, fd, pHdrBuf);

ExitHandler:

	if (pHdrBuf)
	{
		free(pHdrBuf);
	}

	if (fd > 0)
	{
		close(fd);
	}

	LibXmMaQue_Flash_close();
	
	//bXmMaQue_Watchdog_enable(1);

	return res;
}

/********************** XmMaQueSdkDemo -u <Image File>, End. *************************/
