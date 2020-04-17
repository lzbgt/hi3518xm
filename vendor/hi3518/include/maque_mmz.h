
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_mmz.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.04.04
 * 功能描述: 声明MMZ区内存申请和释放相关的接口。有些场合（比如，视频或图片编码）需要用到
 * 		大块内存，并且需要用到虚拟和物理一致的内存，因此预留了MMZ（Memory Management Zone）内
 *		存区，有效防止了编码对系统内存的影响。这些内存区跟Linux操作系统所用的OS Mem无关联，但
 *		可以通过内存映射方式使用。目前需要用到该套内存分配和释放接口模块有：YUV转Jpeg编码。
 *
 * 修改历史:  
 ** 1.日期: 2019.04.04
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.04.04
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_MMZ_H
#define MAQUE_MMZ_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_U32 nMallocLen;		/* 需申请的内存块大小。由调用者传入。*/
	XM_VOID *pPhyAddr;  	/* 从MMZ(Memory Management Zone)分配的物理内存起始地址。底层返回。*/
	XM_VOID *pVirtAddr;		/* pPhyAddr通过内存映射所得的虚拟内存起始地址。底层返回。*/
	XM_HANDLE handleMem;	/* 内存分配handle，供库内部使用，外部调用者只需要把该handle按原值传入。底层返回。*/
	XM_U8 aReserved[8]; 	/* 保留。*/
} MaQueMmzMalloc_s;


/*
【功能描述】
	从预留的MMZ（Memory Management Zone）申请内存块。MMZ跟Linux操作系统所用的OS Mem无关联，但可以
	通过内存映射方式使用。本API从MMZ区申请一块内存，完成映射后，返回物理和虚拟内存地址。
【参数】
	  参数名称					描述
	pstMmzMalloc [in/out]	从MMZ区申请内存的参数。调用者给成员变量nMallocLen赋值，其它成员变量由底层赋值返回。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
【注意】
	YUV转Jpeg编码中，YUV数据通过LibXmMaQue_JpegEnc_Yuv_encode()，传给底层编码器。存放YUV数据的Buffer就是
	通过本API申请所得，因此要注意pstMmzMalloc->nMallocLen的大小计算。它跟YUV帧真实宽高和像素格式有关。
	假如宽高分别为：nWidth和nHeight，像素格式为MAQUE_YUV_PIXEL_FORMAT_YVU_SP420，那么pstMmzMalloc->nMallocLen
	为：（nWidth * nHeight）* 3 / 2。注意：实际大于该值也是允许的，只不过多浪费内存。
*/
XM_S32 LibXmMaQue_MMZ_malloc(MaQueMmzMalloc_s *pstMmzMalloc);

/*
【功能描述】
	释放由前面LibXmMaQue_MMZ_malloc()申请的内存块。两者的参数相同。
【参数】
	  参数名称				描述
	pstMmzMalloc [in]	前面介绍的API：LibXmMaQue_MMZ_malloc()的返回同名结构体对象。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MMZ_free(MaQueMmzMalloc_s *pstMmzMalloc);


#ifdef __cplusplus
}
#endif

#endif
