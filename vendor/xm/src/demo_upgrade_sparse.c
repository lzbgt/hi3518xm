
/*************************************************
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_upgrade_sparse.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.05.27
 * 功能描述: 解析ext4 sparse格式，并写入eMMC相应分区中。
 *
 * 修改历史:  
 ** 1.日期: 2019.05.27
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.05.27
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#include "demo_inc.h"

#include "demo_upgrade_sparse.h"


#define EMMC_BLKSIZE_SHIFT           (9)
#define SZ_1M_SHIFT                  (20)
#define EXT4_BUFFER_SIZE             (64 * 1024)

#define DEBUG_PRINTF_EXT4INFO        1

//extern XM_S32 Demo_Upgrade_updateOneBlock(XM_UL flashAddr, XM_U8 *pData, XM_S32 len);

/******************************************************************************/
static XM_VOID UpgradeSparse_printHeader(SparseHeader_s *pstSparseHdr)
{
#if DEBUG_PRINTF_EXT4INFO
    SIMPLE_PRINTF("sparse header info:\n");
    SIMPLE_PRINTF( "   magic: 0x%x\n",pstSparseHdr->magic);
    SIMPLE_PRINTF( "   major_version: 0x%x\n",pstSparseHdr->major_version);
    SIMPLE_PRINTF( "   minor_version: 0x%x\n",pstSparseHdr->minor_version);
    SIMPLE_PRINTF( "   file_hdr_sz: %d\n",pstSparseHdr->file_hdr_sz);
    SIMPLE_PRINTF( "   chunk_hdr_sz: %d\n",pstSparseHdr->chunk_hdr_sz);
    SIMPLE_PRINTF( "   blk_sz: %d\n",pstSparseHdr->blk_sz);
    SIMPLE_PRINTF( "   total_blks: %d\n",pstSparseHdr->total_blks);
    SIMPLE_PRINTF( "   total_chunks: %d\n",pstSparseHdr->total_chunks);
    SIMPLE_PRINTF( "   image_checksum: %d\n",pstSparseHdr->image_checksum);
#endif
}

/******************************************************************************/
static XM_VOID UpgradeSparse_printChunkInfo(ChunkHeader_t *pstChunkHdr)
{
#if DEBUG_PRINTF_EXT4INFO
    SIMPLE_PRINTF( "chunk header info:\n");
    SIMPLE_PRINTF( "   chunk_type: 0x%x\n",pstChunkHdr->chunk_type);
    SIMPLE_PRINTF( "   chunk_sz: %d\n",pstChunkHdr->chunk_sz);
    SIMPLE_PRINTF( "   total_sz: %d\n",pstChunkHdr->total_sz);
#endif
}

static XM_S32 UpgradeSparse_updateOneBlock(XM_UL flashAddr, XM_U8 *pData, XM_S32 len)
{
	XM_S32 res;
	XM_U8 *pBuf;
	
	if (len <= 0 || len > EXT4_BUFFER_SIZE)
	{
		ERROR_PRINTF("Invalid len: %d, must be (0, %d].\n", len, EXT4_BUFFER_SIZE);
		return XM_SUCCESS;
	}

	//blockSize = Demo_Upgrade_getRdWrBlockSize();
	
	pBuf = (XM_U8 *)malloc(len);
	if (!pBuf)
	{
		ERROR_PRINTF("malloc pBuf Failed: %d.\n", len);
		return XM_FAILURE;
	}

	res = LibXmMaQue_Flash_read(pBuf, flashAddr, len);
	if(res != len)
	{
		free(pBuf);
		ERROR_PRINTF("LibXmMaQue_Flash_read Failed, res: %d, != len: %d.\n", res, len);
		return XM_FAILURE;
	}

	if (!memcmp(pData, pBuf, len))
	{
		free(pBuf);
		INFO_PRINTF ("No need to upgrade the flash addr: 0x%lX.\n", flashAddr);
		return XM_SUCCESS;
	}
		
	res = LibXmMaQue_Flash_write(pData, flashAddr, len);
	if(res != len)
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


XM_S32 Demo_Upgrade_Sparce_writeExt4Sp(const XM_CHAR *pFilename, XM_U64 offset, XM_U64 flashAddrStart) 
{
    XM_U32 i;
	XM_S64 res;
    XM_U64 dense_len = 0;
    XM_U64 unsparse_len = 0;
    XM_U64 show_point = 0;
    XM_U64 show_point_after = 0;
    //XM_U64 img_size;
    XM_U64 chunk_len;
    XM_S64 temp_len;
    XM_U64 img_offset = offset;
    XM_U64 ext4usp_offset = 0;
    XM_U32 count;
    ChunkHeader_t stChunkHdr;
    SparseHeader_s stSparseHdr;
	FILE *pFile;
    XM_U8 *pReadBuf = NULL;
    size_t readsize;
	XM_U64 flashAddr = flashAddrStart;

    pFile = fopen(pFilename, "rb");
    if (!pFile) 
	{
        ERROR_PRINTF("Can't open %s, Err: %s\n", pFilename, strerror(errno));
        return -1;
    }

	res = fseeko(pFile, offset, SEEK_SET);
	if (res != 0)
	{
        ERROR_PRINTF("fseeko() Failed, pFilename: %s, offset: %lld, Err: %s\n", pFilename, offset, strerror(errno));
        res = -1;
		goto ExitHandler;
    }

	pReadBuf = (XM_U8 *)malloc(EXT4_BUFFER_SIZE);
	if (!pReadBuf)
	{
		ERROR_PRINTF("malloc() Failed, EXT4_BUFFER_SIZE: %d\n", EXT4_BUFFER_SIZE);
		res = -1;
		goto ExitHandler;
	}

    img_offset = offset;
    ext4usp_offset = 0;

    readsize = fread(&stSparseHdr, 1, sizeof(stSparseHdr), pFile);
    if (readsize != sizeof(stSparseHdr)) 
	{
        ERROR_PRINTF("fread fail: %d\n ", ferror(pFile));
        res = -1;
		goto ExitHandler;
    }

    img_offset += stSparseHdr.file_hdr_sz; // to the first chunk

    UpgradeSparse_printHeader((SparseHeader_s *)&stSparseHdr);

    if (!XM_IS_SPARSE(((SparseHeader_s *)&stSparseHdr))) 
	{
        ERROR_PRINTF("Not sparse ext4: %s\n", pFilename);
        res = -1;
		goto ExitHandler;
    }

    if (stSparseHdr.blk_sz & ((1 << EMMC_BLKSIZE_SHIFT) - 1)) 
	{
        ERROR_PRINTF("Block size is not aligned to 512Byte: %d.\n", stSparseHdr.blk_sz);
        res = -1;
		goto ExitHandler;
    }

    dense_len = 0;
    unsparse_len = 0;

    for (i = 0; i < stSparseHdr.total_chunks ; i++) 
	{
        if (fseeko(pFile, img_offset, SEEK_SET) != 0)
		{
		 	ERROR_PRINTF("fseeko() Failed, img_offset: %lld <111>\n", img_offset);
        }
		
        readsize = fread(&stChunkHdr, 1, sizeof(stChunkHdr), pFile);
        if (readsize != sizeof(stChunkHdr)) 
		{
            ERROR_PRINTF("fread() stChunkHdr Failed, Err: %d\n", ferror(pFile));
            res = -1;
			goto ExitHandler;
        }

        img_offset += stSparseHdr.chunk_hdr_sz; // to chunk_data or the next chunk header if the chunk is don't care

        UpgradeSparse_printChunkInfo((ChunkHeader_t *)&stChunkHdr);

        switch (stChunkHdr.chunk_type) 
		{
	        case CHUNK_TYPE_RAW:
	            chunk_len = stChunkHdr.chunk_sz * stSparseHdr.blk_sz;
				
	            if (stChunkHdr.total_sz != (chunk_len + stSparseHdr.chunk_hdr_sz)) 
	            {
	                ERROR_PRINTF("Invalid chunk.total_sz: %d != chunk_len: %lld + stSparseHdr.chunk_hdr_sz: %d.\n", 
						stChunkHdr.total_sz, chunk_len, stSparseHdr.chunk_hdr_sz);
	                UpgradeSparse_printChunkInfo((ChunkHeader_t *)&stChunkHdr);
	                res = -1;
					goto ExitHandler;
	            }


	            temp_len = chunk_len;

	            while (temp_len > 0) 
				{
	                count = temp_len;
	                if (count > EXT4_BUFFER_SIZE) 
					{
	                    count = EXT4_BUFFER_SIZE;
	                }

	                //memset(pReadBuf, 0x00, EXT4_BUFFER_SIZE);
					
	                if (fseeko(pFile, img_offset, SEEK_SET) != 0)
	                {
		 				ERROR_PRINTF("fseeko() Failed, img_offset: %lld <222>\n", img_offset);
        			}
					
	                readsize = fread(pReadBuf, 1, count, pFile);
	                if (readsize != count) 
					{
	                    ERROR_PRINTF("fread chunk_data fail, img_offset = 0x%llx, Err: %d\n", img_offset, ferror(pFile));
	                    res = -1;
						goto ExitHandler;
	                }
					
	                res = UpgradeSparse_updateOneBlock(flashAddr, pReadBuf, count);
	                if (res != XM_SUCCESS) 
					{
	                    ERROR_PRINTF("UpgradeSparse_updateOneBlock() Failed, flashAddr: 0x%llX, count: %d <1>.\n", flashAddr, count);
	                    res = -1;
						goto ExitHandler;
	                }

					flashAddr += count;
	
	                img_offset += count;
	                temp_len -= count;
	                ext4usp_offset += count;
	                unsparse_len += count;
	                dense_len += count;

	                show_point = (dense_len >> SZ_1M_SHIFT);
	                if (show_point != show_point_after) 
					{
	                    show_point_after = show_point;
	                    SIMPLE_PRINTF(". ");
	                }

	            }

	            break;

	        case  CHUNK_TYPE_DONT_CARE:
				
	            chunk_len = stChunkHdr.chunk_sz * stSparseHdr.blk_sz;
				
	            if (stChunkHdr.total_sz != stSparseHdr.chunk_hdr_sz) 
	            {
	                ERROR_PRINTF("Invalid stChunkHdr.total_sz: %d != stSparseHdr.chunk_hdr_sz: %d\n",
						stChunkHdr.total_sz, stSparseHdr.chunk_hdr_sz);
	                UpgradeSparse_printChunkInfo((ChunkHeader_t *)&stChunkHdr);
	                res = -1;
					goto ExitHandler;
	            }

	            temp_len = chunk_len;
	            while (temp_len > 0) 
				{
	                count = temp_len;
					
	                if (count > EXT4_BUFFER_SIZE) 
					{
	                    count = EXT4_BUFFER_SIZE;
	                }

	                memset(pReadBuf, 0x00, EXT4_BUFFER_SIZE);

	                res = UpgradeSparse_updateOneBlock(flashAddr, pReadBuf, count);
	                if (res != XM_SUCCESS) 
					{
	                    ERROR_PRINTF("UpgradeSparse_updateOneBlock() Failed, flashAddr: 0x%llX, count: %d <2>.\n ", flashAddr, count);
	                    res = -1;
						goto ExitHandler;
	                }
					
					flashAddr += count;

	                temp_len -= count;
	                ext4usp_offset += count;
	            }

#if DEBUG_PRINTF_EXT4INFO
	            SIMPLE_PRINTF("\nunspare_len=0x%llx,ext4usp_offset=0x%llx\n", unsparse_len,ext4usp_offset);
#endif

	            unsparse_len += chunk_len;

	            break;

	        case  CHUNK_TYPE_FILL:
	        default:
	            break;
        }
    }

    SIMPLE_PRINTF("\ndense_len = 0x%llX(%lldMB), unsparse_len =0x%llX(%lldMB)\n",
		dense_len, dense_len >> SZ_1M_SHIFT, unsparse_len, unsparse_len >> SZ_1M_SHIFT);

   	res = 0;
	
ExitHandler:

	if (pFile)
    {
    	fclose(pFile);
	}

	if (pReadBuf)
	{
		free(pReadBuf);
	}
    return res;
}
