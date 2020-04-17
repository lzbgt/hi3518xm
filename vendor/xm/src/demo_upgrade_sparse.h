
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


#ifndef DEMO_UPGRADE_SPARSE_H
#define DEMO_UPGRADE_SPARSE_H


typedef struct 
{
	XM_U32 magic;           /* 0xed26ff3a */
	XM_U16 major_version;   /* (0x1) - reject images with higher major versions */
	XM_U16 minor_version;   /* (0x0) - allow images with higer minor versions */
	XM_U16 file_hdr_sz;     /* 28 bytes for first revision of the file format */
	XM_U16 chunk_hdr_sz;    /* 12 bytes for first revision of the file format */
	XM_U32 blk_sz;          /* block size in bytes, must be a multiple of 4 (4096) */
	XM_U32 total_blks;      /* total blocks in the non-sparse output image */
	XM_U32 total_chunks;    /* total chunks in the sparse input image */
	XM_U32 image_checksum;  /* CRC32 checksum of the original data, counting "don't care" */
				/* as 0. Standard 802.3 polynomial, use a Public Domain */
				/* table implementation */
} SparseHeader_s;

#define SPARSE_HEADER_MAGIC     0xed26ff3a

#define CHUNK_TYPE_RAW          0xCAC1
#define CHUNK_TYPE_FILL         0xCAC2
#define CHUNK_TYPE_DONT_CARE    0xCAC3

typedef struct chunk_header {
	XM_U16 chunk_type;    /* 0xCAC1 -> raw; 0xCAC2 -> fill; 0xCAC3 -> don't care */
	XM_U16 reserved1;
	XM_U32 chunk_sz;      /* in blocks in output image */
	XM_U32 total_sz;      /* in bytes of chunk input file including chunk header and data */
} ChunkHeader_t;

XM_S32 Demo_Upgrade_Sparce_writeExt4Sp(const XM_CHAR *pFilename, XM_U64 offset, XM_U64 flashAddrStart);

#define SPARSE_HEADER_MAJOR_VER               1

#define XM_IS_SPARSE(_sparse_hdr) \
	(_sparse_hdr->magic == SPARSE_HEADER_MAGIC \
		&& _sparse_hdr->major_version == SPARSE_HEADER_MAJOR_VER \
		&& _sparse_hdr->file_hdr_sz == sizeof(SparseHeader_s) \
		&& _sparse_hdr->file_hdr_sz == sizeof(SparseHeader_s))

#endif /* __SPARSE_H */

