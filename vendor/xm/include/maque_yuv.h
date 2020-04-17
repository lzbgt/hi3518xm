
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_yuv.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义YUV获取相关数据结构，声明相关对外接口。
 *		可通过查询或注册回调方式获取。注意，回调和查询方式不能同时使用，只有最先
 *		使能的方式有效。通过LibXmMaQue_YUV_create()设定初始的通道大小，通过
 *		LibXmMaQue_YUV_setParam()修改。而LibXmMaQue_YUV_getFrame()不会修改通道大小。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_YUV_H
#define MAQUE_YUV_H


#ifdef __cplusplus
extern "C" {
#endif

#define YUV_WIDTH_ALIGN			32
#define YUV_WIDTH_ALIGN_MASK	0x1F
#define YUV_HEIGHT_ALIGN		1
#define YUV_HEIGHT_ALIGN_MASK	0x1

typedef enum 
{
   MAQUE_YUV_PIXEL_FORMAT_YVU_SP420,   /* SP420格式，先保存所有Y数据，再VU交叉存储，VU和Y的宽度相同，高度只有Y的一半。*/
   MAQUE_YUV_PIXEL_FORMAT_YUV_SP420,   /* SP420格式，先保存所有Y数据，再UV交叉存储，UV和Y的宽度相同，高度只有Y的一半。*/
} MaQueYuvPixelFormat_e;

typedef struct
{
	XM_S32 nWidth;  /* 创建YUV获取任务时，指定初始宽。如果为0，将取副码流通道对应VPSS通道输出宽。*/
	XM_S32 nHeight;	/* 创建YUV获取任务时，指定初始高。如果为0，将取副码流通道对应VPSS通道输出高。*/
	XM_U8  aReserved[8]; /* 保留。*/	
} MaQueYuvCreate_s;

typedef struct
{
	XM_S32 nWidth;   /* 返回获取的YUV数据实际宽。它不会设置到YUV输出通道。*/
	XM_S32 nHeight;   /* 返回获取的YUV数据实际高。它不会设置到YUV输出通道。*/
	XM_U8 *pBuffer;   /* 指向获取的YUV数据Buffer指针。*/
	XM_S32 nBufLen;   /* 给pBuffer分配的内存大小。单位：字节。*/
	XM_S32 nDataLen;  /* 实际YUV数据大小。*/
	MaQueYuvPixelFormat_e ePixelFormat;  /* YUV数据存储格式。*/
	XM_U64 pts;    /* YUV数据帧的显示时间戳，单位:微秒。*/
	XM_HANDLE handleMem;  /* 由底层分配内存时，内存由前面介绍的LibXmMaQue_Mem_init()函数注册的pMemAllocFuncPtr
						   * 申请得到，因此需要返回一个Memory Handle给调用者，用于释放。这个Memory Handle就
						   * 保存在本成员中。注：该成员变量，只有由底层分配内存时有效。 */
	XM_U8  aReserved[8];  /* 保留。*/
} MaQueYuvFrame_s;

typedef XM_S32 (*MaQueYuvGetCallbackFuncPtr) (XM_VOID *pUserArg, MaQueYuvFrame_s *pstYuvFrame);

typedef struct 
{
	MaQueYuvGetCallbackFuncPtr pCallbackFuncPtr;  /* 获取YUV数据帧的回调函数指针。*/
	XM_VOID *pCallbackArg;	 /* 传入的私有参数，将会传给pCallbackFuncPtr的第一个参数。*/
} MaQueCallback_s;

typedef struct
{
	MaQueCallback_s stCallback;  /* 用于注册获取YUV数据帧的回调函数指针及注册者私有参数。*/
	XM_U32 nWidth;    /* 设置所抓取YUV的宽。0：将取副通道宽；超过主通道的最大宽，将取主通道的宽。*/
	XM_U32 nHeight;   /* 设置所抓取YUV的高。0：将取副通道高；超过主通道的最大高，将取主通道的高。*/
	XM_U8 aReserved[8]; /* 保留。*/
} MaQueYuvParam_s;

typedef struct
{
	XM_S32 channel;  /* 抓取的通道索引，取值[0, 1]，分别对应两路YUV的通道0和1。*/
	XM_U32 nWidth;   /* 需要抓取的YUV数据实际宽。*/
	XM_U32 nHeight;  /* 需要抓取的YUV数据实际高。*/
	MaQueYuvPixelFormat_e ePixelFormat;  /* YUV图像帧的像素存储格式。*/
	XM_U64 pts;		 /* YUV图像帧的显示时间戳，单位：微秒。*/
	XM_U64 phyAddr;  /* 保存YUV图像帧数据的物理地址。*/
	XM_U32 reserved;   /* 保留。暂时没有用。*/
	XM_VOID *pYuvBuffer;  /* 保存YUV图像帧数据的虚拟地址，即应用层可以访问。*/
	XM_U32 aStride[3];   /* 对应YUV图像帧数据的Y，U，V各分量跨度，即每行在内存中所占用的字节数。*/
	XM_S32 index;     /* 帧缓存的索引。创建时，传入帧缓存数，底层会创建相应内存池，用于帧缓存。这个
					   * 索引用于标识当前帧使用了哪个帧缓存，用于后续释放。帧缓存数，请参考：
					   *	MaQue2YuvCreateParam_s的nCachedFrameCount。
					   */
	XM_HANDLE handleMem; /* 底层从MMZ区分配了内存，所分配的内存信息存放在这个结构体中。主要用于和
						  * Jpeg编码共享内存，即和MaQueJpegFrame_s的handleMem一样，可以互相赋值。
						  * 这样可以利用Jpeg编码接口，实现YUV转Jpeg，可以做到只编局部。
						  * 参考：demo_2yuv2jpeg.c。
						  */
} MaQue2YuvFrame_s;

typedef struct
{
	XM_U32 nWidth;
	XM_U32 nHeight;
	XM_U32 nCachedFrameCount;
} MaQue2YuvCreateParam_s;

typedef XM_VOID (*MaQue2YuvGetCallbackFuncPtr)(MaQue2YuvFrame_s *pstYuvFrame1, MaQue2YuvFrame_s *pstYuvFrame2, XM_VOID *pUserData);

typedef struct 
{
	MaQue2YuvGetCallbackFuncPtr pCallbackFuncPtr;  /* 获取2个YUV数据帧的回调函数指针。*/
	XM_VOID *pCallbackArg;	 /* 传入的私有参数，将会传给pCallbackFuncPtr的第一个参数。*/
} MaQue2YuvCallback_s;

typedef struct
{
	MaQue2YuvCreateParam_s astCreateParam[2];
	XM_U32 fps;
	MaQue2YuvCallback_s stCallback;  /* 用于注册获取YUV数据帧的回调函数指针及注册者私有参数。*/
	XM_U8 aReserved[8]; /* 保留。*/
} MaQue2YuvCreate_s;


/*
【功能描述】
	创建获取YUV数据的任务。
【参数】
	  参数名称				描述
	channel [in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstYuvCreate [in] 	用于创建获取YUV数据任务的参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_YUV_create(XM_S32 channel, MaQueYuvCreate_s *pstYuvCreate);

/*
【功能描述】
	改变YUV数据宽高等参数，也可以通过本API注册获取YUV数据的回调函数。

【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstYuvParam [in]	设置YUV数据获取参数（宽高或回调函数）。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	前面LibXmMaQue_YUV_create()函数传入参数pstYuvCreate指定YUV宽高。LibXmMaQue_YUV_setParam()
	可修改宽高。下面介绍的LibXmMaQue_YUV_getFrame()第二个参数pstYuvFrame，也会涉及到宽高，这个
	不会修改YUV输出通道宽高，它只返回YUV数据的实际宽高。
*/
XM_S32 LibXmMaQue_YUV_setParam(XM_S32 channel, MaQueYuvParam_s *pstYuvParam);

/*
【功能描述】
	查询方式，获取一帧YUV数据。

【参数】
	  参数名称				描述
	channel [in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstYuvFrame	[out] 	所获取YUV数据帧信息。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	pstYuvFrame中指定的宽高，不会设置到YUV输出通道，它只保存YUV数据的实际宽高。要修改
	YUV通道的宽高，应调用前面介绍的API：LibXmMaQue_YUV_setParam()。
*/

XM_S32 LibXmMaQue_YUV_getFrame(XM_S32 channel, MaQueYuvFrame_s *pstYuvFrame);


/*
【功能描述】
	销毁由LibXmMaQue_YUV_create()创建的获取YUV任务。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_YUV_destroy(XM_S32 channel);

/*
【功能描述】
	创建获取两路YUV图像帧的任务。这两路YUV是同源的，也即小图由大图缩小所得。两路YUV图像帧是通过注册下去的回调函数回调上来的。
【原型】
	XM_S32 LibXmMaQue_2YUV_create(XM_S32 channel, MaQue2YuvCreate_s *pst2YuvCreate);
【参数】
	  参数名称					描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstYuvCreate [in]	用于创建获取2路YUV数据任务的参数。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
*/
XM_S32 LibXmMaQue_2YUV_create(XM_S32 channel, MaQue2YuvCreate_s *pst2YuvCreate);

/*
【功能描述】
	释放所获取的两路YUV图像帧的其中一帧。具体释放帧参数包含在pstYuvFrame中。pstYuvFrame是底层回调返回的结构体，原样传入即可。
【原型】
	XM_S32 LibXmMaQue_2YUV_releaseFrame(XM_S32 channel, MaQue2YuvFrame_s *pstYuvFrame);
【参数】
	  参数名称					描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstYuvFrame	[in]	需要释放的YUV图像帧信息。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
*/
XM_S32 LibXmMaQue_2YUV_releaseFrame(XM_S32 channel, MaQue2YuvFrame_s *pstYuvFrame);

/*
【功能描述】
	销毁由LibXmMaQue_2YUV_create()创建的任务。
【原型】
	XM_S32 LibXmMaQue_2YUV_destroy(XM_S32 channel);
【参数】
	  参数名称					描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
【注意】
	如果LibXmMaQue_2YUV_create()创建的任务获取YUV图像帧，用于Jpeg编码，则需要先停止Jpeg编码器，
	方可调用本API，否则本API释放了Jpeg编码共享的VB块，会导致程序崩溃。
*/
XM_S32 LibXmMaQue_2YUV_destroy(XM_S32 channel);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_YUV_H


