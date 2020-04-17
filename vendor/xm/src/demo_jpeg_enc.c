
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_jpeg_encode.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示获取Jpeg编码后的图片。
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
#include "demo_jpeg_enc.h"
#include "demo_mem.h"


#define DEMO_JPEG_ENCODE_DIR	"JpegEncode"
#define DEMO_JPEG_CALLBACK_FILE		"CallbackJpeg#%d_%dx%d.jpg"
#define DEMO_JPEG_LIB_MALLOC_FILE	"LibMallocJpegEncode#%d_%dx%d.jpg"
#define DEMO_JPEG_USER_MALLOC_FILE	"UserMallocJpegEncode#%d_%dx%d.jpg"
#define DEMO_JPEG_YUV2JPEG	"Yuv2JpegEncode#%d_%dx%d.jpg"

#define DEMO_JPEG_READ_SIZE_MAX		(128 * 1024)

typedef enum 
{
	JPEG_BUFFER_TYPE_USER_MALLOC,
	JPEG_BUFFER_TYPE_LIB_MALLOC,
	JPEG_BUFFER_TYPE_CALLBACK,
	JPEG_BUFFER_TYPE_YUV2JPEG,
} JpegBufferType_e;

typedef struct
{
	XM_S32 *pFrameCount;
	XM_S32 bWriteFile;
} JpegGetCallbackArg_s;

typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bWriteFile;
	XM_U32 nFrames;
	XM_S32 nWidth;
	XM_S32 nHeight;
	XM_CHAR aYuvPixFmt[16];
	XM_CHAR aYuvDir[256];
} JpegGetModParam_s;

typedef struct
{
	XM_S32 bEnabled;
	MaQueOsdParam_s stOsdParam;
	XM_CHAR aFontLibPath[128];
	XM_CHAR aTitle[128];
} OsdTitleModParam_s;


static XM_S32 g_bJpegEncStop;
static XM_S32 g_bJpegEncOsdInited;

static XM_VOID Demo_JpegEnc_ModParam_print(XM_CHAR *pOwner, JpegGetModParam_s *pstJpegGetModParam)
{
	SIMPLE_PRINTF("@%s->Demo_JpegEnc_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstJpegGetModParam->bEnabled);
	SIMPLE_PRINTF("\tbWriteFile: %d\n", pstJpegGetModParam->bWriteFile);
	SIMPLE_PRINTF("\tnFrames: %d\n", pstJpegGetModParam->nFrames);
	SIMPLE_PRINTF("\tnWidth: %d\n", pstJpegGetModParam->nWidth);
	SIMPLE_PRINTF("\tnHeight: %d\n", pstJpegGetModParam->nHeight);
	SIMPLE_PRINTF("\taYuvPixFmt: %s\n", pstJpegGetModParam->aYuvPixFmt);
	SIMPLE_PRINTF("\taYuvDir: %s\n", pstJpegGetModParam->aYuvDir);
}

static XM_VOID Demo_JpegEnc_OsdModParam_print(XM_CHAR *pOwner, OsdTitleModParam_s *pstOsdModParam)
{
	SIMPLE_PRINTF("@%s->Demo_VideoEnc_OsdModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstOsdModParam->bEnabled);
	SIMPLE_PRINTF("\tstOsdParam.index: %d\n", pstOsdModParam->stOsdParam.index);
	SIMPLE_PRINTF("\tstOsdParam.left: %d\n", pstOsdModParam->stOsdParam.left);
	SIMPLE_PRINTF("\tstOsdParam.top: %d\n", pstOsdModParam->stOsdParam.top);
	SIMPLE_PRINTF("\tstOsdParam.eFontSize: %d\n", pstOsdModParam->stOsdParam.eFontSize);
	SIMPLE_PRINTF("\tstOsdParam.fgTrans: %d\n", pstOsdModParam->stOsdParam.fgTrans);
	SIMPLE_PRINTF("\tstOsdParam.bgTrans: %d\n", pstOsdModParam->stOsdParam.bgTrans);
	SIMPLE_PRINTF("\tstOsdParam.fgColor: 0x%x\n", pstOsdModParam->stOsdParam.fgColor);
	SIMPLE_PRINTF("\tstOsdParam.bgColor: 0x%x\n", pstOsdModParam->stOsdParam.bgColor);
	SIMPLE_PRINTF("\taFontLibPath: %s\n", pstOsdModParam->aFontLibPath);
	SIMPLE_PRINTF("\taTitle: %s\n", pstOsdModParam->aTitle);
}


static XM_VOID Demo_JpegEnc_printFrame(XM_CHAR *pOwner, MaQueJpegFrame_s *pstJpegFrame)
{
	SIMPLE_PRINTF("@%s->Demo_Jpeg_printFrame():\n", pOwner);
	SIMPLE_PRINTF("\tnWidth: %d\n", pstJpegFrame->nWidth);
	SIMPLE_PRINTF("\tnHeight: %d\n", pstJpegFrame->nHeight);
	SIMPLE_PRINTF("\tpBuffer: 0x%lx\n", (XM_UL)pstJpegFrame->pBuffer);
	SIMPLE_PRINTF("\tnBufLen: %d\n", pstJpegFrame->nBufLen);
	SIMPLE_PRINTF("\tnDataLen: %d\n", pstJpegFrame->nDataLen);
	SIMPLE_PRINTF("\tPts: %lld\n", pstJpegFrame->pts);
}

static XM_S32 Demo_JpegEnc_makeFilePath(XM_CHAR *pFilePath, JpegBufferType_e eBufType, XM_S32 frameSeq, XM_S32 nWidth, XM_S32 nHeight)
{
	XM_S32 res;
	XM_CHAR aTmpFile[256];
	XM_CHAR *pEnv = NULL;
	XM_CHAR *pDir = DEMO_JPEG_ENCODE_DIR;

	pEnv = getenv("MAQUE_RECORD_STORE_DIR");
	if (pEnv)
	{
		strcpy(aTmpFile, pEnv);
		INFO_PRINTF("pEnv: %s.\n", pEnv);
		strcat(aTmpFile, "/");
		strcat(aTmpFile, DEMO_JPEG_ENCODE_DIR);
		pDir = aTmpFile;
	}

	res = mkdir(pDir, 0755);
	if (res != 0 && EEXIST != errno)
	{
		ERROR_PRINTF("mkdir Failed: %s, Error: %s!\n", pDir, strerror(errno));
		return XM_FAILURE;
	}
	else
	{
		INFO_PRINTF("mkdir OK: %s!\n", pDir);
	}

	strcpy(pFilePath, pDir);
	strcat(pFilePath, "/");

	switch(eBufType)
	{
		case JPEG_BUFFER_TYPE_USER_MALLOC:
			
			sprintf(aTmpFile, DEMO_JPEG_USER_MALLOC_FILE, frameSeq, nWidth, nHeight);
			
			break;

		case JPEG_BUFFER_TYPE_LIB_MALLOC:
			
			sprintf(aTmpFile, DEMO_JPEG_LIB_MALLOC_FILE, frameSeq, nWidth, nHeight);
			
			break;

		case JPEG_BUFFER_TYPE_CALLBACK:
			
			sprintf(aTmpFile, DEMO_JPEG_CALLBACK_FILE, frameSeq, nWidth, nHeight);
			
			break;

		case JPEG_BUFFER_TYPE_YUV2JPEG:

			sprintf(aTmpFile, DEMO_JPEG_YUV2JPEG, frameSeq, nWidth, nHeight);
			
			break;

		default:
			
			ERROR_PRINTF("Invalid eBufType: %d!\n", eBufType);
			
			break;
	}

	strcat(pFilePath, aTmpFile);

	INFO_PRINTF("pFilePath: %s!\n", pFilePath);

	return XM_SUCCESS;
}

static XM_S32 Demo_JpegEnc_getFrame_callback (XM_VOID *pUserArg, MaQueJpegFrame_s *pstJpegFrame)
{
	XM_S32 res;
	JpegGetCallbackArg_s *pstCallbackArg = (JpegGetCallbackArg_s *)pUserArg;

	//INFO_PRINTF("stJpegParam.nDataLen: %d, WH[%d, %d], pts: %lld <bWriteFile: %d>!\n", 
	//		pstJpegFrame->nDataLen, pstJpegFrame->nWidth, pstJpegFrame->nHeight, pstJpegFrame->pts, pstCallbackArg->bWriteFile);
	
	if (pstCallbackArg->bWriteFile)
	{
		XM_CHAR aFileName[256];
				
		res = Demo_JpegEnc_makeFilePath(aFileName, JPEG_BUFFER_TYPE_CALLBACK, *pstCallbackArg->pFrameCount, 
					pstJpegFrame->nWidth, pstJpegFrame->nHeight);
		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("Demo_JpegEnc_makeFilePath Failed!\n");
		}
		else
		{
			MaQue_Demo_File_writeSimply(aFileName, pstJpegFrame->pBuffer, pstJpegFrame->nDataLen);
		}
	}
	else
	{
		Demo_JpegEnc_printFrame("Demo_Jpeg_getFrame_callback()", pstJpegFrame);
	}

	(*pstCallbackArg->pFrameCount) += 1;

	MaQue_Demo_Mem_release(pstJpegFrame->handleMem);

	return XM_SUCCESS;
}

static XM_VOID *Demo_JpegEnc_getFrameByCallback_task(XM_VOID *pArg)
{
	XM_S32 res;
	MaQueJpegCallbackParam_s stCallbackParam;
	XM_S32 nFrames = 0;
	JpegGetCallbackArg_s stCallbackArg;
	JpegGetModParam_s stJpegGetModParam = { 0 };
	StringValueGet_s astJpegGetValGet[] = 
	{
		{"bEnabled:", &stJpegGetModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bEnabled)},
		{"bWriteFile:", &stJpegGetModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bWriteFile)},
		{"nFrames:", &stJpegGetModParam.nFrames, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nFrames)},	
		{"nWidth:", &stJpegGetModParam.nWidth, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nWidth)},	
		{"nHeight:", &stJpegGetModParam.nHeight, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nHeight)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByCallback", astJpegGetValGet, sizeof(astJpegGetValGet) / sizeof(StringValueGet_s));

	if (!stJpegGetModParam.bEnabled)
	{
		INFO_PRINTF("Jpeg Get Module Not Enabled!\n");	
		return NULL;
	}

	Demo_JpegEnc_ModParam_print("GetJpegByCallback", &stJpegGetModParam);

	if (LibXmMaQue_JpegEnc_create(0, NULL))  /* pstCreate参数暂时没有启用，可传NULL。*/
	{
		ERROR_PRINTF("LibXmMaQue_JpegEnc_create err!\n");	
		return NULL;
	}

	memset(&stCallbackParam, 0, sizeof(MaQueJpegCallbackParam_s));

	stCallbackParam.nWidth = stJpegGetModParam.nWidth;
	stCallbackParam.nHeight = stJpegGetModParam.nHeight;

	stCallbackArg.pFrameCount = &nFrames;
	stCallbackArg.bWriteFile = stJpegGetModParam.bWriteFile;
		
	stCallbackParam.stCallback.pCallbackFuncPtr = Demo_JpegEnc_getFrame_callback;
	stCallbackParam.stCallback.pCallbackArg = &stCallbackArg;
			
	res = LibXmMaQue_JpegEnc_registerCallback(0, &stCallbackParam);
	if (res != XM_SUCCESS) 
	{
		ERROR_PRINTF("LibXmMaQue_JpegEnc_registerCallback err!\n");	
		goto ExitHandler;
	}

	if (stJpegGetModParam.nFrames <= 0)
	{
		stJpegGetModParam.nFrames = (XM_U32)-1;
	}

	INFO_PRINTF("---------------- Jpeg Get Frames Start <Callback> -----------------!\n");	

	while(!g_bJpegEncStop && nFrames <= stJpegGetModParam.nFrames)
	{
		usleep(10*1000);
	}

	stCallbackParam.stCallback.pCallbackFuncPtr = NULL;
	LibXmMaQue_JpegEnc_registerCallback(0, &stCallbackParam);

	INFO_PRINTF("---------------- Jpeg Get Frames End, nFrames: %d <Callback> -----------------!\n", nFrames);

	res = XM_SUCCESS;

ExitHandler:

	LibXmMaQue_JpegEnc_destroy(0);
	
	if (g_bJpegEncOsdInited)
	{
		LibXmMaQue_OSD_destroy();
	}

	return NULL;
}

static XM_S32 Demo_JpegEnc_getOneFrame(XM_S32 seq, XM_S32 bLibMalloc, 
					XM_U8 *pBuf, XM_S32 bufSize, JpegGetModParam_s *pstModParam)
{
	MaQueJpegFrame_s stJpegFrame;

	INFO_PRINTF("<Enter>!\n");

	if (LibXmMaQue_JpegEnc_create(0, NULL))  /* pstCreate参数暂时没有启用，可传NULL。*/
	{
		ERROR_PRINTF("LibXmMaQue_JpegEnc_create err!\n"); 
		return 0;
	}

	memset(&stJpegFrame, 0x0, sizeof(MaQueJpegFrame_s));

	stJpegFrame.nWidth = pstModParam->nWidth;
	stJpegFrame.nHeight = pstModParam->nHeight;

	if (!bLibMalloc)
	{
		stJpegFrame.pBuffer = pBuf;
		stJpegFrame.nBufLen = bufSize;
	}
	
	if (LibXmMaQue_JpegEnc_getFrame(0, &stJpegFrame) == XM_SUCCESS)
	{
		INFO_PRINTF("stJpegParam.nDataLen: %d, WH[%d, %d], pts: %lld <bWriteFile: %d>!\n", 
			stJpegFrame.nDataLen, stJpegFrame.nWidth, stJpegFrame.nHeight, stJpegFrame.pts, pstModParam->bWriteFile);
		
		if(stJpegFrame.nDataLen > 0)
		{
			if (pstModParam->bWriteFile)
			{
				XM_S32 res;
				XM_CHAR aFileName[256];
				
				res = Demo_JpegEnc_makeFilePath(aFileName, bLibMalloc ? JPEG_BUFFER_TYPE_LIB_MALLOC : JPEG_BUFFER_TYPE_USER_MALLOC, 
							seq, stJpegFrame.nWidth, stJpegFrame.nHeight);
				if (res != XM_SUCCESS)
				{
					ERROR_PRINTF("Demo_JpegEnc_makeFilePath Failed!\n");
					return XM_FAILURE;
				}
		
				MaQue_Demo_File_writeSimply(aFileName, stJpegFrame.pBuffer, stJpegFrame.nDataLen);
			}
			else
			{
			}

			if (bLibMalloc)
			{
				MaQue_Demo_Mem_release(stJpegFrame.handleMem);
			}
		}
	}
	else
	{
		ERROR_PRINTF("LibXmMaQue_JpegEnc_getJpeg() Failed <001>!\n");
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_JpegEnc_getFrameByMalloc(XM_S32 bLibMalloc)
{
	XM_S32 nFrames = 0;
	XM_U8 *pBuf = NULL;
	XM_S32 bufSize = 0;
	JpegGetModParam_s stJpegGetModParam = { 0 };
	StringValueGet_s astJpegGetValGet[] = 
	{
		{"bEnabled:", &stJpegGetModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bEnabled)},
		{"bWriteFile:", &stJpegGetModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bWriteFile)},
		{"nFrames:", &stJpegGetModParam.nFrames, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nFrames)},	
		{"nWidth:", &stJpegGetModParam.nWidth, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nWidth)},	
		{"nHeight:", &stJpegGetModParam.nHeight, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nHeight)},
	};

	if (bLibMalloc)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByLibMalloc", astJpegGetValGet, sizeof(astJpegGetValGet) / sizeof(StringValueGet_s));

		Demo_JpegEnc_ModParam_print("GetJpegByLibMalloc", &stJpegGetModParam);
	}
	else
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByUserMalloc", astJpegGetValGet, sizeof(astJpegGetValGet) / sizeof(StringValueGet_s));

		Demo_JpegEnc_ModParam_print("GetJpegByUserMalloc", &stJpegGetModParam);
	}
	
	if (!stJpegGetModParam.bEnabled)
	{
		INFO_PRINTF("Jpeg Get Module Not Enabled!\n");	
		return XM_SUCCESS;
	}

	if (!bLibMalloc)
	{
		bufSize = (stJpegGetModParam.nWidth * stJpegGetModParam.nHeight) >> 2;
		pBuf = malloc(bufSize);
		if (!pBuf)
		{
			ERROR_PRINTF("Malloc pBuf Failed, bufSize: %d!\n", bufSize);	
			return XM_SUCCESS;
		}
	}

	if (stJpegGetModParam.nFrames <= 0)
	{
		stJpegGetModParam.nFrames = (XM_U32)-1;
	}

	INFO_PRINTF("---------------- Jpeg Get Frames Start <Malloc>-----------------!\n");	
	
	while(!g_bJpegEncStop && nFrames < stJpegGetModParam.nFrames)
	{
		Demo_JpegEnc_getOneFrame(nFrames, bLibMalloc, pBuf, bufSize, &stJpegGetModParam);
		nFrames++;
	}

	if (!bLibMalloc)
	{
		free(pBuf);
	}

	INFO_PRINTF("---------------- Jpeg Get Frames End, nFrames: %d <Malloc>-----------------!\n", nFrames);	

	return XM_SUCCESS;
}

static XM_VOID *Demo_JpegEnc_getFrame_task(XM_VOID *pArg)
{
	XM_S32 bLibMalloc = (XM_S32)pArg;

	INFO_PRINTF("bLibMalloc: %d!\n", bLibMalloc);	
	
	Demo_JpegEnc_getFrameByMalloc(bLibMalloc);

	return NULL;
}

static XM_S32 Demo_JpegEnc_yuv2Jpeg(MaQueMmzMalloc_s *pstMmzMalloc, XM_S32 seq, XM_U8 *pBuf, XM_S32 dataSize, JpegGetModParam_s *pstModParam)
{
	MaQueJpegFrame_s stJpegFrame;

	INFO_PRINTF("<Enter>!\n");
	
	memset(&stJpegFrame, 0x0, sizeof(MaQueJpegFrame_s));

	stJpegFrame.nWidth = pstModParam->nWidth;
	stJpegFrame.nHeight = pstModParam->nHeight;
	stJpegFrame.eYuvPixFmt = !strcasecmp(pstModParam->aYuvPixFmt, "YVU_SP420") 
		? MAQUE_YUV_PIXEL_FORMAT_YVU_SP420 : MAQUE_YUV_PIXEL_FORMAT_YUV_SP420;
	
	stJpegFrame.pBuffer = pBuf;  /* pBuf 就是 pstMmzMalloc->pVirtAddr */
	stJpegFrame.nBufLen = pstMmzMalloc->nMallocLen;
	stJpegFrame.nDataLen = dataSize;
	stJpegFrame.handleMem = pstMmzMalloc->handleMem;

	if (LibXmMaQue_JpegEnc_Yuv_encode(0, &stJpegFrame) == XM_SUCCESS)
	{
		INFO_PRINTF("stJpegParam.nDataLen: %d, WH[%d, %d], pts: %lld <bWriteFile: %d>!\n", 
			stJpegFrame.nDataLen, stJpegFrame.nWidth, stJpegFrame.nHeight, stJpegFrame.pts, pstModParam->bWriteFile);
		
		if(stJpegFrame.nDataLen > 0)
		{
			if (pstModParam->bWriteFile)
			{
				XM_S32 res;
				XM_CHAR aFileName[256];
				
				res = Demo_JpegEnc_makeFilePath(aFileName, JPEG_BUFFER_TYPE_YUV2JPEG, 
							seq, stJpegFrame.nWidth, stJpegFrame.nHeight);
				if (res != XM_SUCCESS)
				{
					ERROR_PRINTF("Demo_JpegEnc_makeFilePath Failed!\n");
					return XM_FAILURE;
				}
		
				MaQue_Demo_File_writeSimply(aFileName, stJpegFrame.pBuffer, stJpegFrame.nDataLen);
			}
			else
			{
			}
		}
	}
	else
	{
		ERROR_PRINTF("LibXmMaQue_JpegEnc_getJpeg() Failed <001>!\n");
	}

	return XM_SUCCESS;
}

static XM_VOID *Demo_JpegEnc_Yuv_task(XM_VOID *pArg)
{
	XM_S32 i = 0, res;
	XM_U8 *pBuf = NULL;
	XM_U8 *pDat = NULL;
	XM_S32 bufSize = 0;
	JpegGetModParam_s stJpegGetModParam = { 0 };
	StringValueGet_s astJpegGetValGet[] = 
	{
		{"bEnabled:", &stJpegGetModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bEnabled)},
		{"bWriteFile:", &stJpegGetModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.bWriteFile)},
		{"nFrames:", &stJpegGetModParam.nFrames, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nFrames)},	
		{"nWidth:", &stJpegGetModParam.nWidth, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nWidth)},	
		{"nHeight:", &stJpegGetModParam.nHeight, XM_VALUE_TYPE_INT, sizeof(stJpegGetModParam.nHeight)},
		{"YuvPixFmt:", &stJpegGetModParam.aYuvPixFmt, XM_VALUE_TYPE_STRING, sizeof(stJpegGetModParam.aYuvPixFmt)},
		{"YuvDir:", &stJpegGetModParam.aYuvDir, XM_VALUE_TYPE_STRING, sizeof(stJpegGetModParam.aYuvDir)},
	};
	struct dirent **pstDirEntList;
	XM_S32 fileCount = 0;
	FILE *pFile;
	MaQueJpegEncCreate_s stCreate = { 0 };
	XM_CHAR *pEnv = NULL;
	XM_CHAR *pDir = NULL;
	MaQueMmzMalloc_s stMmzMalloc = { 0 };
	
	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"Yuv2Jpeg", astJpegGetValGet, sizeof(astJpegGetValGet) / sizeof(StringValueGet_s));

	Demo_JpegEnc_ModParam_print("Yuv2Jpeg", &stJpegGetModParam);

	if (!stJpegGetModParam.bEnabled)
	{
		INFO_PRINTF("Jpeg Get Module Not Enabled!\n");	
		return NULL;
	}

	if ((stJpegGetModParam.nWidth & 0x1) || (stJpegGetModParam.nHeight & 0x1)) /* YUV转Jpeg编码要求宽高都为2的倍数。如果不是，自行先做处理*/
	{
		INFO_PRINTF("nWidth: %d or nHeight: %d, must be aligned to 2!\n", stJpegGetModParam.nWidth, stJpegGetModParam.nHeight);	
		return NULL;
	}

	bufSize = ((stJpegGetModParam.nWidth * stJpegGetModParam.nHeight) * 3 ) >> 1;

	stMmzMalloc.nMallocLen = bufSize;
	
	res = LibXmMaQue_MMZ_malloc(&stMmzMalloc);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_MMZ_malloc Failed, bufSize: %d!\n", bufSize);	
		return NULL;
	}

	pBuf = (XM_U8 *)stMmzMalloc.pVirtAddr;

	stCreate.nWidth = stJpegGetModParam.nWidth;
	stCreate.nHeight = stJpegGetModParam.nHeight;

	if (LibXmMaQue_JpegEnc_Yuv_create(0, &stCreate))  /* pstCreate参数暂时没有启用，可传NULL。*/
	{
		LibXmMaQue_MMZ_free(&stMmzMalloc);
		
		ERROR_PRINTF("LibXmMaQue_JpegEnc_Yuv_create err!\n"); 
		return NULL;
	}

	pDir = stJpegGetModParam.aYuvDir;

	pEnv = getenv("MAQUE_YUV2JPEG_YUV_DIR");
	if (pEnv)
	{
		pDir = pEnv;
		INFO_PRINTF("pEnv: %s.\n", pEnv);
	}
	
	fileCount = MaQue_Demo_Dir_getDirOrFileName(pDir, DIR_ENTRY_TYPE_FILE, &pstDirEntList, NULL);

	INFO_PRINTF("---------------- Jpeg Get Frames Start <Malloc>-----------------!\n");	
	
	while(!g_bJpegEncStop && i < fileCount)
	{
		XM_S32 fileSize;
		XM_S32 readTotalSize = 0;
		XM_CHAR aFilePath[256];

		sprintf(aFilePath, "%s/%s", pDir, pstDirEntList[i]->d_name);
		
		fileSize = MaQue_Demo_File_getSize(aFilePath);
			
		if (fileSize > bufSize)
		{
			ERROR_PRINTF("Too Big fileSize: %d, Must <= bufSize: %d!\n", fileSize, bufSize);	
			break;
		}
		
		pFile = fopen(aFilePath, "rb");
		if (!pFile)
		{
			ERROR_PRINTF("fopen() Failed, %s!\n", aFilePath);
			break;
		}

		pDat = pBuf;

		while ((res = fread(pDat, 1, DEMO_JPEG_READ_SIZE_MAX, pFile)) > 0)
		{
			pDat += res;
			readTotalSize += res;
		}

		fclose(pFile);

		if (readTotalSize != bufSize)
		{
			INFO_PRINTF("readTotalSize: %d != bufSize: %d!\n", readTotalSize, bufSize);	
		}
		
		Demo_JpegEnc_yuv2Jpeg(&stMmzMalloc, i, pBuf, readTotalSize, &stJpegGetModParam);
		i++;
	}

	LibXmMaQue_JpegEnc_Yuv_destroy(0);

	MaQue_Demo_Dir_freeDirEntryList(pstDirEntList, fileCount);

	LibXmMaQue_MMZ_free(&stMmzMalloc);

	INFO_PRINTF("---------------- Jpeg Get Frames End, nFrames: %d <Malloc>-----------------!\n", i);	

	return XM_SUCCESS;
}

static XM_S32 Demo_JpegEnc_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bJpegEncStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_JpegEnc_Osd_overlap(XM_VOID)
{
	OsdTitleModParam_s stOsdTimeModParam = { 0 };
	OsdTitleModParam_s stOsdTitleModParam = { 0 };
	StringValueGet_s astOsdTitleValGet[] = 
	{
		{"bEnabled:", &stOsdTitleModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.bEnabled)},
		{"Index:", &stOsdTitleModParam.stOsdParam.index, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.index)},	
		{"Left:", &stOsdTitleModParam.stOsdParam.left, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.left)},	
		{"Top:", &stOsdTitleModParam.stOsdParam.top, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.top)},	
		{"FontSize:", &stOsdTitleModParam.stOsdParam.eFontSize, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.eFontSize)},
		{"FgTrans:", &stOsdTitleModParam.stOsdParam.fgTrans, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.fgTrans)},
		{"BgTrans:", &stOsdTitleModParam.stOsdParam.bgTrans, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.bgTrans)},
		{"FgColor:", &stOsdTitleModParam.stOsdParam.fgColor, XM_VALUE_TYPE_HEX, sizeof(stOsdTitleModParam.stOsdParam.fgColor)},
		{"BgColor:", &stOsdTitleModParam.stOsdParam.bgColor, XM_VALUE_TYPE_HEX, sizeof(stOsdTitleModParam.stOsdParam.bgColor)},
		{"FontLibPath:", &stOsdTitleModParam.aFontLibPath, XM_VALUE_TYPE_STRING, sizeof(stOsdTitleModParam.aFontLibPath)},
		{"Title:", &stOsdTitleModParam.aTitle, XM_VALUE_TYPE_STRING, sizeof(stOsdTitleModParam.aTitle)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"OsdTimeJpegChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	stOsdTimeModParam = stOsdTitleModParam;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"OsdTitleJpegChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));

	Demo_JpegEnc_OsdModParam_print("stOsdTimeModParam", &stOsdTimeModParam);
	Demo_JpegEnc_OsdModParam_print("stOsdTitleModParam", &stOsdTitleModParam);

	if (stOsdTitleModParam.bEnabled)
	{
		LibXmMaQue_OSD_showTime(0, MAQUE_STREAM_CHN_JPEG, &stOsdTimeModParam.stOsdParam);
		
		if (stOsdTitleModParam.aFontLibPath[0] 
			&& strncasecmp(stOsdTitleModParam.aFontLibPath, "NULL", strlen("NULL")))
		{
			XM_S32 i;

			INFO_PRINTF("aFontLibPath: %s!\n", stOsdTitleModParam.aFontLibPath);

			LibXmMaQue_OSD_init(stOsdTitleModParam.aFontLibPath);
			
			for (i = 0; i < 1; i++)
			{
				LibXmMaQue_OSD_setTitle(0, MAQUE_STREAM_CHN_JPEG, &stOsdTitleModParam.stOsdParam, stOsdTitleModParam.aTitle);
				usleep(100 *1000);
			}
	
			g_bJpegEncOsdInited = 1;
		}
	}	

	return XM_SUCCESS;
}
XM_S32 MaQue_Demo_JpegEnc_start(XM_VOID)
{
	XM_S32 bEnabled = 0, nEnabledCount = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_JpegEnc_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);

	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByCallback", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_JpegEnc_getFrameByCallback_task, (XM_VOID *)1, "JpegEnc_ByCallback");
			nEnabledCount++;
		}
	}
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByLibMalloc", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_JpegEnc_getFrame_task, (XM_VOID *)1, "JpgEnc_LibMalloc");
			nEnabledCount++;
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetJpegByUserMalloc", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_JpegEnc_getFrame_task, (XM_VOID *)0, "JpgEnc_UsrMalloc");
			nEnabledCount++;
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Yuv2Jpeg", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_JpegEnc_Yuv_task, (XM_VOID *)0, "JpgEnc_Yuv");
			nEnabledCount++;
		}
	}

	if (nEnabledCount > 0)
	{
		Demo_JpegEnc_Osd_overlap();
	}

	return XM_SUCCESS;
}


