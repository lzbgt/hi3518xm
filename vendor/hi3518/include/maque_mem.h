
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_mem.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明注册内存分配函数指针的接口。
 *		给底层注册内存分配和释放接口。视频和Jpeg编码器输出的码流大小不一，都是分段的，要拼成
 *		一帧完整数据，拷贝是不可避免的，并且往往需要分配大块内存。最简单想法是，底层编码输出
 *		线程根据实际码流大小，调用malloc函数分配内存，上层负责释放，但这往往太理想化了，因为
 *		内存紧张时，申请大块内存将会十分困难，延时大，甚至失败；反复申请和释放内存，必将造成
 *		碎片化严重，进一步恶化。MaQue实现时，充分考虑了这些因素，提供了一套接口，由二次开发者
 *		实现自己内存分配和释放优化程序。开发者可以在刚开始跑App内存很充裕的时候，申请大片内存
 *		用作内存池，写一个既方便，又高效的内存管理器，例如，环形缓存区管理器。如果需要用到物理
 *		内存的，还可以用物理和虚拟内存映射实现。
 *		目前需要用到该套内存分配和释放接口模块由MaQueMemoryType_e决定。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_MEM_H
#define MAQUE_MEM_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	MAQUE_MEM_TYPE_VIDEO_ENC = 1,   /* 视频编码。*/
	MAQUE_MEM_TYPE_JPEG_ENC,  /* Jpeg编码。*/
	MAQUE_MEM_TYPE_YUV_GET	  /* YUV获取。*/
} MaQueMemoryType_e;

typedef struct
{
	MaQueMemoryType_e eMemType;  /* 内存申请的类型。指明所申请的内存用于哪个模块。*/
	XM_S32 subType;   /* 对应于eMemType的子类型。eMemType只指定了一个大类，例如Video Enc，实际视频
					   * 编码又有主码流和副码流之分，可用该变量加以区分。目前，暂只用于视频编码，
					   * subType对应于 MaQueStreamChannel_e。
					   */
	XM_U32 nBufSize;  /* 需要申请的Buffer大小。*/
	XM_U32 nAlign;    /* 申请内存的起始地址对齐字节数。*/
	XM_U8 *pBuffer;   /* 指向申请到的内存起始地址。*/
	XM_U8 aReserved[8];  /* 保留。*/
} MaQueMemAllocParam_s;

typedef XM_S32 (*MaQueMemoryAllocFuncPtr)(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam);
typedef XM_S32 (*MaQueMemoryReleaseFuncPtr)(XM_HANDLE handle);
typedef XM_S32 (*MaQueMemoryAddRefFuncPtr)(XM_HANDLE handle);
typedef XM_S32 (*MaQueMemorySetLengthFuncPtr)(XM_HANDLE handle, XM_U32 len);

typedef struct 
{
	MaQueMemoryAllocFuncPtr  pMemAllocFuncPtr;   /* 申请内存回调。*/
	MaQueMemoryReleaseFuncPtr pMemReleaseFuncPtr;  /* 释放内存回调。如果使用参考数机制，则先把参考数减1，等参考数为0时，释放内存。*/
	MaQueMemoryAddRefFuncPtr pMemAddRefFuncPtr;  /* 给内存使用参考数加1。*/
	MaQueMemorySetLengthFuncPtr pMemSetLenFuncPtr;  /* 设置申请的内存实际使用量。*/
} MaQueMemoryApi_s;


/*
【功能描述】
	给底层注册内存分配和释放接口。目前应用于视频和Jpeg编码输出，以及YUV数据获取。
【参数】
	  参数名称			描述
	pstMemApi [in]	注册给底层的内存分配和释放接口。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	目前需要用到该套内存分配和释放接口的模块由MaQueMemoryType_e决定，也即上层必须
	实现MaQueMemoryType_e定义的所有类型内存分配。
*/
XM_S32 LibXmMaQue_Mem_init(MaQueMemoryApi_s *pstMemApi); 

#ifdef __cplusplus
}
#endif


#endif //MAQUE_MEM_H


