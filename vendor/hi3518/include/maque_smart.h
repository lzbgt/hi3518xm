
#ifndef MAQUE_SMART_H
#define MAQUE_SMART_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAQUE_MAX_CLASS_NUM 2
#define MAQUE_MAX_RECT_NUM 10

typedef enum
{
	MAQUE_SMART_ROI_MB_MODE_4X4,
	MAQUE_SMART_ROI_MB_MODE_8X8,
	MAQUE_SMART_ROI_MB_MODE_16X16,
	MAQUE_SMART_ROI_MB_MODE_BUTT
} MaQueSmartRoiMbMode_e;

typedef struct
{
	XM_S32 		bEnabled; 		/* 人形、人脸使能开关 0 : 关闭；1 : 开启*/
	XM_FLOAT 	pdThreshold; 	/* 阈值，范围[0.0,1.0)，值越大SMD 告警、人形检测越不灵敏。 */
	XM_FLOAT 	fdThreshold;	/* 阈值，范围[0.0,1.0)，值越大人脸检测越不灵敏。 */
	XM_S32 		bRoiEnabled;	/* 感兴趣区域调节开关 */
	XM_U32		roiThreshold; 	/* 阈值，取值范围[1,1024]。 值越大，感兴趣区域的 SMD 告警、人形检测或人脸检测越不灵敏。 */
	MaQueSmartRoiMbMode_e roiMbMode; 	/* 感兴趣区域宏块模式*/
	XM_U32 		imgWidth; 		/* 感兴趣区域处理图像的宽，固定640 */
	XM_U32 		imgHeight; 		/* 感兴趣区域处理图像的高 ，固定360*/
	XM_U8 		*pMbMap; 		/* 宏块映射表.for 4x4 mb mode, alloc (DIV_UP(img_width,4))*(DIV_UP(img_height,4)) bytes */
} MaQueSmartParams_s;

typedef struct
{
    XM_S16 s16X1;        /**< 左上角x坐标 */
    XM_S16 s16Y1;        /**< 左上角y坐标 */
    XM_S16 s16X2;        /**< 右下角x坐标 */
    XM_S16 s16Y2;        /**< 右下角y坐标 */
} MaQueSmartRect_s;

typedef struct
{
	XM_S32 targetPDNum; 					//人形目标个数
	XM_S32 targetFDNum; 					//人脸目标个数
	MaQueSmartRect_s aPDRect[MAQUE_MAX_RECT_NUM];	  	//人形目标坐标
	MaQueSmartRect_s aFDRect[MAQUE_MAX_RECT_NUM];	  	//人脸目标坐标
	XM_S32 aReserved[8];
} MaQueSmartTarget_s;

typedef struct
{
    XM_S32 bLowBitrate; 	/* 智能PQ 低码率使能开关*/
	XM_S32 bIsoAdaptive; 	/* 基于 ISO 的质量码率自适应调节开关*/
	XM_S32 aReserved[6];
} MaQueSmartLowBitRate_s;

typedef struct
{
	XM_U32 nWidth;    /* 所抓取图片的宽。*/
	XM_U32 nHeight;   /* 所抓取图片的宽。*/
	XM_U8 *pBuffer;
	XM_S32 nBufLen;   /* 如果nBufLen > 0，将由调用者给pBuffer分配内存；nBufLen<= 0，
						由底层分配内存，调用者负责释放。目前仅支持底层分配内存的方式。*/
	XM_S32 nDataLen;  /* 返回实际Jpeg码流大小。*/
	XM_U64 pts;	
	XM_HANDLE handleMem;
	XM_S32 nToltalJpeg;	/* 小图个数。*/
	XM_S32 nIndex;		/* 小图索引，从0开始。*/
	XM_CHAR aClassName[4]; /* 人形: pd，人脸: fd ，大图 : pic */
	XM_U8 aReserved[4]; /* 保留。*/
} MaQueSmartJpegFrame_s;

typedef XM_S32 (*MaQueSmartGetJpegCallbackFuncPtr) (XM_VOID *pUserArg, MaQueSmartJpegFrame_s *pstJpegFrame);

typedef struct 
{
	MaQueSmartGetJpegCallbackFuncPtr pCallbackFuncPtr;
	XM_VOID *pCallbackArg;	
} MaQueSmartJpegCallback_s;

typedef enum
{
	MAQUE_SMART_JPEG_ENCODE_LARGE,		/// 整帧图片。
	MAQUE_SMART_JPEG_ENCODE_CUTOUT,		/// 整帧图片中的人脸或者人形小图。
	MAQUE_SMART_JPEG_ENCODE_NR,
} MaQueSmartJpegEncodeType_e;

typedef enum
{
	MAQUE_SMART_JPEG_CLASS_NONE = 0x0,
	MAQUE_SMART_JPEG_CLASS_PD = 0x1,
	MAQUE_SMART_JPEG_CLASS_FD = 0x2,
	MAQUE_SMART_JPEG_CLASS_NR,
} MaQueSmartJpegClass_e;

typedef struct
{
	MaQueSmartJpegCallback_s stCallback;
	MaQueSmartJpegEncodeType_e eType;
	MaQueSmartJpegClass_e eClass; // 小图类型，大图改参数无效。人形和人脸小图同时编码: MAQUE_SMART_JPEG_CLASS_PD|MAQUE_SMART_JPEG_CLASS_FD
	XM_S32 aReserved[7]; /* 保留。*/
} MaQueSmartJpegCallbackParam_s;

/*
【功能描述】
	创建人脸、人形功能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartCreate(XM_S32 channel);

/*
【功能描述】
	获取人脸、人形目标个数及坐标信息，坐标信息是相对(640 * 360 ) 的坐标体系。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstMaQueSmartTarget [out]  输出结果，参考: MaQueSmartTarget_s。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartGetTarget(XM_S32 channel, MaQueSmartTarget_s *pstMaQueSmartTarget);

/*
【功能描述】
	配置人脸、人形检测参数。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstMaQueSmartParams [in] 配置参数，参考: pstMaQueSmartParams 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartSetParameter(XM_S32 channel, MaQueSmartParams_s *pstMaQueSmartParams);

/*
【功能描述】
	销毁人脸、人形功能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartDestory(XM_S32 channel);

/*
【功能描述】
	配置低码率功能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in] 大小码流，支持各码流单独配置，值参考: MaQueStreamChannel_e 。
	pstMaQueSmartLowBitRate [in] 低码率配置参数。参考: MaQueSmartLowBitRate_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartLowBitRate(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueSmartLowBitRate_s *pstMaQueSmartLowBitRate);

/*
【功能描述】
	配置智能PQ 功能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bEnable [in]	0 : 失能，1 : 使能。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartAdvanceIsp(XM_S32 channel, XM_S32 bEnable);

/*
【功能描述】
	人形检测阈值配置。与LibXmMaQue_SmartSetParameter 中的pdThreshold 是一个效果，此处可单独动态配置。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	threshold [in]	人形检测阈值。默认0.75，推荐调节范围0.65 ~ 0.85 之间。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartPDThreshold(XM_S32 channel, XM_FLOAT threshold);

/*
【功能描述】
	人脸检测阈值配置。与LibXmMaQue_SmartSetParameter 中的fdThreshold 是一个效果，此处可单独动态配置。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	threshold [in]	人脸检测阈值。默认0.63，推荐调节范围0.6 ~ 0.8 之间。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartFDThreshold(XM_S32 channel, XM_FLOAT threshold);

/*
【功能描述】
	注册人脸抓图数据回调，一张图片触发一次回调。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	MaQueSmartFdJpegCallbackParam_s [in]	回调参数，参考MaQueSmartFdJpegCallbackParam_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_SmartRegisterCallback(XM_S32 channel, MaQueSmartJpegCallbackParam_s *pstCallback);

#ifdef __cplusplus
}
#endif


#endif //MAQUE_IDE_H

