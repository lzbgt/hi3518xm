
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_jpeg_enc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义Jpeg编码相关结构体，声明Jpeg编码相关接口，仅外部使用。
 *		提供了创建Jpeg编码，获取Jpeg编码帧等接口。Jpeg编码输入的数据有来自Sensor输出VI输入，
 *    也有由上层调用者直接传入YUV数据帧。下文描述以"编Sensor输出VI输入图像帧"和"编上层传入的
 *    YUV图像帧"来区分。两者共用一套结构体，因此传参方式类似，但具体成员变量含义却有明显差别，
 *    具体参考下面解析。"编上层传入的YUV图像帧"API都以"LibXmMaQue_JpegEnc_Yuv_"作为前缀。
 *    编Sensor输出VI输入图像帧：
 *           编码产生的Jpeg图像帧，可通过查询或注册回调方式获取。注意，回调和查询方式可以同时
 *       使用。查询方式，每次都可以修改图像宽高，而回调函数只在调用注册函数时设置一次，因此，
 *       当回调和查询同时使用时，回调返回的图像大小为最后一次设置到编码通道的大小，也就是，当
 *       注册回调后，再调用查询方式，如修改了宽高，那么回调返回的宽高也随之改变。
 *           Jpeg编码帧获取函数，都会传宽高下去，修改编码通道大小。而下面介绍的YUV数据获取（参
 *       考：maque_yuv.h章节），只有LibXmMaQue_YUV_create()和LibXmMaQue_YUV_setParam()可修改通
 *		 道大小，而获取函数不会修改通道大小。
 *	  编上层传入的YUV图像帧：
 *		 由调用者传入YUV数据帧及其信息，编码完成后，通过同一个结构体返回Jpeg数据及其信息。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_JPEG_ENCODE_H
#define MAQUE_JPEG_ENCODE_H


#include "maque_struct.h"
#include "maque_yuv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_S32 nWidth; /* 设置图像编码实际宽。编Sensor输出VI输入图像帧：nWidth或nHeight其一为0，都将取客户定制的最大抓图宽高。
	                * 编上层传入的YUV图像帧：
					*	nWidth或nHeight必须不小于YUV帧真实宽高，即不能为0。大于YUV帧真实宽高是允许的，即创建比YUV帧真实
					*   宽高还要大的编码器也是可以的，只不过多消耗资源。
	                */
	XM_S32 nHeight;  /* 设置图像编码实际高。编Sensor输出VI输入图像帧：nWidth或nHeight其一为0，都将取客户定制的最大抓图宽高。
	                  *  编上层传入的YUV图像帧：
					  *	  nWidth或nHeight必须不小于YUV帧真实宽高，即不能为0。大于YUV帧真实宽高是允许的，即创建比YUV帧真实
					  *   宽高还要大的编码器也是可以的，只不过多消耗资源。
	                  */
	XM_U8 aReserved[8];  /* 保留。*/
} MaQueJpegEncCreate_s;

typedef struct
{
	XM_U32 nWidth;    /* 编Sensor输出VI输入图像帧：
	                   *        用于查询方式抓图时，设置所抓取图片的宽。设置为0或超过，都将取编码器创建时支持的最大宽度。
					   *    查询或回调方式，最终该变量会保存实际Jpeg图像宽返回。即底层可能会修改该变量值。
					   * 编上层传入的YUV图像帧：
					   *    传入时，设置YUV帧的真实宽。返回编码实际宽，如果YUV宽是2对齐的，则传入和返回宽一致。
					   */
	XM_U32 nHeight;   /* 编Sensor输出VI输入图像帧：
	                   *	    用于查询方式抓图时，设置所抓取图片的高，设置为0或超过，都将取编码器创建时支持的最大高度。
					   *    查询或回调方式，最终该变量会保存实际Jpeg图像高返回。即底层可能会修改该变量值。
					   * 编上层传入的YUV图像帧：
					   *	传入时，设置YUV帧的真实高。返回编码实际高，如果YUV高是2对齐的，则传入和返回高一致。
					   */
	XM_U8 *pBuffer;   /* 编Sensor输出VI输入图像帧：保存Jpeg编码图像帧数据。可以由调用者分配，也可以由底层分配。
					   * 编上层传入的YUV图像帧： 传入是YUV数据，返回是编码后的Jpeg数据。必须是LibXmMaQue_MMZ_malloc()申请
					   * 所得的虚拟地址。
					   */
	XM_S32 nBufLen;   /* 编Sensor输出VI输入图像帧：
	                   *       给pBuffer分配的内存大小，单位：字节。如果nBufLen > 0，同时 pBuffer非NULL，说明由调用者给
					   *    pBuffer分配内存；否则，由底层分配内存，调用者负责释放。
					   *       注：由底层分配内存时，底层是调用由 LibXmMaQue_Mem_init()注册下去的内存分配接口实现的，因此
					   *    需要该套机制释放。具体参考：maque_mem.h章节。
					   *  编上层传入的YUV图像帧：传入是YUV数据长度，底层不会修改。
					   */
	XM_S32 nDataLen;  /* 编Sensor输出VI输入图像帧：返回实际Jpeg码流大小。
					   * 编上层传入的YUV图像帧：传入是YUV数据长度，传出是编码后Jpeg数据长度。
					   */
	XM_U64 pts;	      /* 编Sensor输出VI输入图像帧：Jpeg编码时系统时间，单位：微秒。
					   * 编上层传入的YUV图像帧：
					   *	 可以传入时间戳，也可以传入0，由底层赋编码时的时间戳（单位都为微秒）。
					   */
	XM_HANDLE handleMem;  /* 编Sensor输出VI输入图像帧：
	                       *       由底层分配内存时，内存由下文介绍的LibXmMaQue_Mem_init()函数注册的pMemAllocFuncPtr申请
						   *   得到，因此需要返回一个Memory Handle给调用者，用于释放。这个Memory Handle就保存在本成员
						   *   中。注：该成员变量，只有由底层分配内存时有效。
						   * 编上层传入的YUV图像帧：LibXmMaQue_MMZ_malloc()返回的pstMmzMalloc->handleMem，它只供LibXmMaQue
						   *	库内部使用，调用者只需按原值传入即可。
						   */
	MaQueYuvPixelFormat_e eYuvPixFmt;  /* 编Sensor输出VI输入图像帧：没有用。
										* 编上层传入的YUV图像帧：YUV数据帧的像素格式。
										*/
	MaQueRect_s stCropRect;  /* 用于Jpeg裁剪，即只编码局部图像。只要width和height其一为0，则不起作用。 */
	XM_U8 aReserved[8]; /* 保留。*/
} MaQueJpegFrame_s;

typedef XM_S32 (*MaQueJpegGetCallbackFuncPtr) (XM_VOID *pUserArg, MaQueJpegFrame_s *pstJpegFrame);

typedef struct 
{
	MaQueJpegGetCallbackFuncPtr pCallbackFuncPtr;  /* 获取Jpeg编码数据的回调函数。*/
	XM_VOID *pCallbackArg;	 /* 注册回调函数时，传入的注册者私有结构体。将会在调用回调函数时，传给第一个参数。
							  * 只有注册者使用，底层不关心具体内容，因此可为NULL。
							  */
} MaQueJpegCallback_s;

typedef struct
{
	MaQueJpegCallback_s stCallback;  /* 通过回调方式获取Jpeg编码输出帧，需注册的回调函数指针和注册者私有结构体。*/
	XM_U32 nWidth;    /* 设置图像编码实际宽。nWidth或nHeight其一为0，都将取客户定制的最大抓图宽高。*/
	XM_U32 nHeight;   /* 设置图像编码实际高。 nWidth或nHeight其一为0，都将取客户定制的最大抓图宽高。*/
	XM_U8 aReserved[8]; /* 保留。*/
} MaQueJpegCallbackParam_s;

/*
【功能描述】
	创建Jpeg硬编码器。参考下面【注意】说明，该API只是返回成功。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstCreate [in]	创建Jpeg编码器参数。目前暂时不需要设置，可传NULL。
【返回值】
	XM_SUCCESS	成功。
	其它	失败，请参考错误码。
【注意】
	Jpeg编码器是硬件模块实现的。给底层编码器预分配的内存大小有限，防止反复销毁或创建，
	造成内存碎片，导致最后无法再创建成功，因此，Jpeg编码器在LibXmMaQue_System_startUp()时，
	就按客户需求的最大分辨率（跟sensor有关）创建。只要编码图像大小变化不大于创建时最大
	分辨率，就可以不用销毁编码器。
*/
XM_S32 LibXmMaQue_JpegEnc_create(XM_S32 channel, MaQueJpegEncCreate_s *pstCreate);

/*
【功能描述】
	注册回调函数，获取Jpeg编码图像帧。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstCallbackParam [in]	注册给底层获取Jpeg编码图像帧的回调函数，同时设置图像的宽高。宽或
							高其一为0，按创建编码器时设定的最大分辨率编码。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	注册回调方式获取Jpeg编码帧，要求Jpeg编码一直在运行，系统开销很大，同时要求调用者，及时
	处理每一帧图像，哪怕丢掉。如果阻塞，会导致底层内存块得不到释放，最终不光影响Jpeg，还会
	影响视频编码（因Jpeg和视频编码有部分内存块是共享使用的）。通过回调方式获取Jpeg编码帧，
	可以很及时地返回每帧图像，适用于连续抓图。如果不需连续抓图，建议用接下来将要介绍的查询
	方式获取，请参考API：LibXmMaQue_JpegEnc_getFrame()。
	只需调用本API一次，注册过回调函数后，底层每编好一帧图像，都会传给回调函数。要想停止回调
	方式获取Jpeg编码帧，只需要再调用一次本API，把注册的回调函数指针置成NULL。
*/
XM_S32 LibXmMaQue_JpegEnc_registerCallback(XM_S32 channel, MaQueJpegCallbackParam_s *pstCallbackParam);

/*
【功能描述】
	通过查询方式，获取Jpeg编码图像帧。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstJpegFrame [int/out]	获取Jpeg编码图像帧的参数。可以设置图像的宽高；可以由调用者传入分配
							好的Buffer，也可以由底层按实际编码帧的大小分配Buffer，由调用者负责
							释放Buffer。具体参考：MaQueJpegFrame_s成员说明。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	通过查询方式获取Jpeg编码帧，会设置Jpeg单帧编码模式（即每编码完一帧就暂停），对系统开销
	很小。要连续抓图，请参考前面介绍的API：LibXmMaQue_JpegEnc_registerCallback()。
*/
XM_S32 LibXmMaQue_JpegEnc_getFrame(XM_S32 channel, MaQueJpegFrame_s *pstJpegFrame);

/*
【功能描述】
	销毁Jpeg编码器。目前，暂时不支持Jpeg反复销毁，只是把前面注册的回调清空。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它		失败，请参考错误码。
【注意】
	目前本API实现的功能，相当于调用LibXmMaQue_JpegEnc_registerCallback()，把其中回调函数
	指针设置为NULL。
*/
XM_S32 LibXmMaQue_JpegEnc_destroy(XM_S32 channel);

/*
【功能描述】
	创建YUV转Jpeg硬编码器。
【参数】
	  参数名称			描述	
	channel [in]	对应编码通道。暂只支持一个通道，应设置为0。	
	pstCreate [in]	创建Jpeg编码器参数。结构体中nWidth和nHeight必须不小于YUV帧真实宽高，即不能为0。	
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	与前面LibXmMaQue_JpegEnc_create()相比，传参样式相同，但这里结构体对象pstCreate的nWidth和nHeight
	必须不小于YUV帧真实宽高。大于YUV帧真实宽高是允许的，即创建比YUV帧真实宽高还要大的编码器也是可以
	的，只不过多消耗资源。
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_create(XM_S32 channel, MaQueJpegEncCreate_s *pstCreate);

/*
【功能描述】
	把一帧YUV数据编码成Jpeg图片。
【参数】
	  参数名称				描述
	channel [in]		对应编码通道。暂只支持一个通道，应设置为0。
	pstFrame [in/out]	传入时，保存YUV数据帧及其信息，返回时，保存Jpeg码流及其信息。传入时，结构体中
						的nWidth和nHeight必须设定为YUV真实宽高，返回编码实际宽高，如果YUV宽高是2对齐，
						则传入和返回宽高一致。pBuffer传入是YUV数据，返回是编码后的Jpeg数据。nBufLen传
						入是YUV数据长度，底层不会修改。nDataLen传入是YUV数据长度，传出是编码后Jpeg数据
						长度。pts可以传入时间戳，也可以传入0，由底层赋编码时的时间戳（单位都为微秒）。
						handleMem是LibXmMaQue_MMZ_malloc()返回的pstMmzMalloc-> handleMem。eYuvPixFmt是
						YUV数据帧的像素格式。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	与前面LibXmMaQue_JpegEnc_getFrame()相比，传参样式一样，但结构体对象pstFrame的成员变量含义差别很大，
	参考"【参数】"表格详细说明。YUV宽高底层会按2对齐处理，所以最好是偶数。YUV宽高参考Jpeg编码器限制，
	最小为32。
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_encode(XM_S32 channel, MaQueJpegFrame_s *pstFrame);

/*
【功能描述】
	销毁YUV转Jpeg编码器。
【参数】
	  参数名称			描述
	channel [in]	对应编码通道。暂只支持一个通道，应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_destroy(XM_S32 channel);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_JPEG_ENCODE_H


