
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_video_enc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义视频编码相关数据结构，声明相关对外接口。
 *		视频编码需要消耗大量内存，因此在调用LibXmMaQue_System_startUp()时，就已经创建
 *		完成，并开始编码。本章提供的API可用在编码器创建完成，并开始编码的过程中。
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_VIDEO_ENC_H
#define MAQUE_VIDEO_ENC_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum  
{
	MAQUE_VIDEO_COMPRESS_DIVX_MPEG4,	///< DIVX MPEG4。
	MAQUE_VIDEO_COMPRESS_MS_MPEG4,		///< MS MPEG4。
	MAQUE_VIDEO_COMPRESS_MPEG2,			///< MPEG2。
	MAQUE_VIDEO_COMPRESS_MPEG1,			///< MPEG1。
	MAQUE_VIDEO_COMPRESS_H263,			///< H.263
	MAQUE_VIDEO_COMPRESS_MJPG,			///< MJPG
	MAQUE_VIDEO_COMPRESS_FCC_MPEG4,		///< FCC MPEG4
	MAQUE_VIDEO_COMPRESS_H264,			///< H.264
	MAQUE_VIDEO_COMPRESS_H265,			///< H.265
	MAQUE_VIDEO_COMPRESS_JPEG,			///< JPEG
	MAQUE_VIDEO_COMPRESS_NR				///< 
} MaQueVideoCompress_e;

/// 捕获分辨率类型
/// \note更新下面这个结构时，要同步更新Record.cpp里验证编码能力
typedef enum 
{
    MAQUE_CAPTURE_SIZE_D1,		///< 720*576(PAL)	720*480(NTSC)
    MAQUE_CAPTURE_SIZE_HD1,		///< 352*576(PAL)	352*480(NTSC)
    MAQUE_CAPTURE_SIZE_BCIF,	///< 720*288(PAL)	720*240(NTSC)
    MAQUE_CAPTURE_SIZE_CIF,		///< 352*288(PAL)	352*240(NTSC)
    MAQUE_CAPTURE_SIZE_QCIF,	///< 176*144(PAL)	176*120(NTSC)
    MAQUE_CAPTURE_SIZE_VGA,		///< 640*480(PAL)	640*480(NTSC)
    MAQUE_CAPTURE_SIZE_QVGA,	///< 320*240(PAL)	320*240(NTSC)
    MAQUE_CAPTURE_SIZE_360P,	///< 640*360(PAL)	640*360(NTSC)
    MAQUE_CAPTURE_SIZE_QQVGA,	///< 160*128(PAL)	160*128(NTSC)
    MAQUE_CAPTURE_SIZE_ND1 = 9,   ///< 240*192
    MAQUE_CAPTURE_SIZE_650TVL,    ///< 928*576
    MAQUE_CAPTURE_SIZE_720P,      ///< 1280*720
    MAQUE_CAPTURE_SIZE_1_3M,      ///< 1280*960
    MAQUE_CAPTURE_SIZE_UXGA ,     ///< 1600*1200
    MAQUE_CAPTURE_SIZE_1080P,     ///< 1920*1080
    MAQUE_CAPTURE_SIZE_WUXGA,     ///< 1920*1200
    MAQUE_CAPTURE_SIZE_2_5M,      ///< 1872*1408
    MAQUE_CAPTURE_SIZE_3M,        ///< 2048*1536
    MAQUE_CAPTURE_SIZE_5M,        ///< 3744*1408
    MAQUE_CAPTURE_SIZE_EXT_NR =19,    ///< 枚举的图形大小种类的数目。
    MAQUE_CAPTURE_SIZE_1080N = 19,    ///< 960*1080
    MAQUE_CAPTURE_SIZE_4M,        	///< 2592*1520
    MAQUE_CAPTURE_SIZE_6M,			///< 3072×2048
    MAQUE_CAPTURE_SIZE_8M,			///< 3264×2448
    MAQUE_CAPTURE_SIZE_12M,			///< 4000*3000
    MAQUE_CAPTURE_SIZE_4K,			///< 4096 * 2160通用/3840*2160海思
    MAQUE_CAPTURE_SIZE_EXT_V2_NR=25,    ///< 枚举的图形大小种类的数目。
    MAQUE_CAPTURE_SIZE_720N = 25,  ///  640*720
    MAQUE_CAPTURE_SIZE_WSVGA,      ///< 1024*576
    MAQUE_CAPTRUE_SIZE_RES2,       // 0*0
    MAQUE_CAPTURE_SIZE_RES3,       // 0*0
    MAQUE_CAPTURE_SIZE_RES4,       // 0*0
    MAQUE_CAPTURE_SIZE_RES5,       // 0*0
    MAQUE_CAPTURE_SIZE_RES6,       // 0*0
    MAQUE_CAPTURE_SIZE_NR,
} MaQueCaptureSize_e;

typedef enum  
{
	MAQUE_BITRATE_CTRL_CBR,			///< 恒定码流
	MAQUE_BITRATE_CTRL_VBR,			///< 可变码流
	MAQUE_BITRATE_CTRL_MBR,			///< 混合码流
	MAQUE_BITRATE_CTRL_NR			///< 
} MaQueBitrateCtrl_e;


/// image quality
typedef enum
{
	MAQUE_IMG_QUALITY_WORST = 1,		///< 最差
	MAQUE_IMG_QUALITY_WORSE = 2,		///< 较差
	MAQUE_IMG_QUALITY_GENERAL = 3,		///< 一般
	MAQUE_IMG_QUALITY_GOOD = 4,			///< 好
	MAQUE_IMG_QUALITY_BETTER = 5,		///< 更好
	MAQUE_IMG_QUALITY_BEST = 6,			///< 最好
	MAQUE_IMG_QUALITY_NR = 7
} MaQueImageQuality_e;

typedef enum 
{
	IFRAME_INTV_TYPE_TIME,
	IFRAME_INTV_TYPE_FRAME_COUNT,
	IFRAME_INTV_TYPE_NR,
} IFrameIntervalType_e;


typedef struct
{
	MaQueCaptureSize_e eCapSizeMax;  /* 视频通道最大编码分辨率。*/
} MaQueVidChnAbility_s;

typedef struct
{
	XM_U64 videoEncTypeMask;  /* 视频编码（即压缩标准）类型掩码，是支持的MaQueCaptureSize_e类型枚举值对应的位移相或所得。
							   *  如：支持H265，则：videoEncTypeMask |= 1 << MAQUE_VIDEO_COMPRESS_H265。
							   */
	XM_U32 maxEncPowerX1080P;  /* 最大编码能力，单位为：1080P，即每秒支持多少帧1080P编码。*/
	MaQueCaptureSize_e eDecImageSizeMax;  /* 支持的最大解码分辨率。如果不支持则为：MAQUE_CAPTURE_SIZE_NR。*/
	MaQueVidChnAbility_s astVidEncChnAbility[MAQUE_STREAM_CHN_NR]; /* 每个视频通道（包含Jpeg编码通道）的编码能力。*/
} MaQueCodeAbilities_s;

typedef struct 
{
	MaQueVideoCompress_e	eVidComp;  /* 视频压缩标准（H264，H265等）。*/
	MaQueCaptureSize_e		eCapSize;	/* 视频分辨率。*/
	MaQueBitrateCtrl_e		eBitrateCtrl;  /* 码流控制类型。*/
	MaQueImageQuality_e		eImageQuality;  /* 编码图像质量。*/
	XM_S32		nFps;					/* 编码帧率。即每秒编多少帧。*/
	XM_S32		nBitRate;				/* 编码比特率。单位：Kb。*/
	IFrameIntervalType_e eIFrmIntvType;  /* I帧间隔，有时间或帧数，具体参考: IFrameIntervalType_e */
	XM_S32		nIFrameInterval;		/* I帧间隔，即配置nIFrameInterval 秒一个I帧或两个I帧间的帧数，根据前一个变量eIFrmIntvType而定。*/
	XM_U32 		bUseUserQp;     /* 是否使用下面指定的Qp值。*/
	XM_S32		iQpMin;			/* 最小QP值，[1，51]。当 bUseUserQp为1时，才有效。注：Qp值在可变码率编码时有用，
								 * 它和编码比特率一起控制图像质量。使用Qp值越小，产生的码流越大，图像质量越好。
								 */
	XM_S32		iQpMax;			/* 最大QP值，[1，51]。当 bUseUserQp为1时，才有效。*/
} MaQueVideoEncodeCfg_s;

#if 0
typedef struct 
{
	XM_S32 nMainAudio;                // Main stream audio flag: 0 - disable 1 - enable
	XM_S32 nExtraVideo;               // Extra stream video flag: 0 - disable 1 - enable
	XM_S32 nExtraAudio;               // Extra stream audio flag: 0 - disable 1 - enable
	MaQueVideoFormat_s stMain;  // Main stream
	MaQueVideoFormat_s stExtra; // Extra stream
	MaQueVideoFormat_s stSnap;  // Snap stream
}MaQueVideoEncodeCfg_s;
#endif

typedef enum
{
	MAQUE_HEADER_TYPE_FRAME_INFO = 1,
	MAQUE_HEADER_TYPE_USER,
} MaQueHeaderType_e;

/// FrameType
typedef enum
{
	MAQUE_FRAME_TYPE_NONE,
	MAQUE_FRAME_TYPE_VIDEO,
	MAQUE_FRAME_TYPE_AUDIO,
	MAQUE_FRAME_TYPE_DATA
} MaQueFrameType_e;


/// FrameSubType
typedef enum
{
	MAQUE_FRAME_SUBTYPE_I = 0,
	MAQUE_FRAME_SUBTYPE_P = 1,
	MAQUE_FRAME_SUBTYPE_B = 2,
	MAQUE_FRAME_SUBTYPE_S = 3,
	MAQUE_FRAME_SUBTYPE_TEXT = 5,  /* Data */
	MAQUE_FRAME_SUBTYPE_TNTL = 6,
} MaQueFrameSubType_e;


/// 码流类型
typedef enum
{
	MAQUE_ENCODE_TYPE_NONE = 0,
	MAQUE_ENCODE_TYPE_H264 = 1,
	MAQUE_ENCODE_TYPE_H265 = 2,
	MAQUE_ENCODE_TYPE_JPEG = 3,
} MaQueEncodeType_e;

typedef struct 
{
	XM_HANDLE handleMem;	/* 参考前面：maque_mem.h章节，调用 pMemAllocFuncPtr返回的pHandle，用于上层释放内存。*/
	XM_U8 *pHeader;			/* 指向视频流帧头数据。暂保留，不用。*/
	XM_U32 nHeaderLen;		/* 视频流帧头数据长度。单位：字节。暂保留，不用。*/ 
	MaQueHeaderType_e eHeaderType;	/* 视频流帧头类型。暂保留，不用。*/
	XM_U8 *pData;            	/* 指向视频码流数据。*/
	XM_U32 nDataLen;         	/* 视频码流数据长度。*/
	XM_U32 nMallocLen;       	/* pData所指Buffer大小。*/
	MaQueFrameType_e eFrameType;     /* 视频码流对应的帧类型（指的是视频，音频，数据等）。*/
	MaQueFrameSubType_e eSubType;    /* 视频码流对应的帧子类型（指的是I，P，B帧等）。*/
	MaQueEncodeType_e eEncodeType;   /* 视频码流对应的编码类型（指的是H264，H265等）。*/
	XM_U32 nFrameRate;			/* 视频帧率。*/
	XM_S32 nWidth;			    /* 视频图像宽。*/
	XM_S32 nHeight;             /* 视频图像高。*/
	
	MaQueTimeStamp_s stTimeStamp;  /* 时间戳。*/
	XM_U64 pts;		/* 显示时间戳，单位：us。*/
	XM_U32 version;		/* 保留。暂不用。*/	
	XM_U8 aReserved[16];	/* 保留。*/
} MaQueVideoEncFrameInfo_s;

typedef XM_S32 (*MaQueVideoGetStreamCallbackFuncPtr) (XM_VOID *pUserArg, MaQueVideoEncFrameInfo_s *pstFrameInfo);

typedef struct 
{
	MaQueRectCoord_s stRectCoord;  /* 视频遮挡块的矩形坐标。*/
	XM_U32 color;    /* 视频遮挡块填充的颜色。*/
	XM_U32 enable;   /* 是否显示视频遮挡块。1：显示；0：隐藏。*/
} MaQueVideoCoverParam_s;

typedef enum
{
	MAQUE_VIDEO_ENC_PROFILE_BASE,
	MAQUE_VIDEO_ENC_PROFILE_MAIN,
	MAQUE_VIDEO_ENC_PROFILE_HIGH,
	MAQUE_VIDEO_ENC_PROFILE_NR,
} MaQueVideoEncProfile_e;


/*
【功能描述】
	图像分辨率名称转换成枚举值。例如："3M"输入，将返回枚举值MAQUE_CAPTURE_SIZE_3M。
【参数】
	  参数名称				描述
	pCapSizeName [in]	图像分辨率名称字符串。
【返回值】
	MaQueCaptureSize_e	分辨率枚举值。不是合法字符串，将返回MAQUE_CAPTURE_SIZE_NR。
【注意】
	图像分辨率枚举值及其对应的名称字符串关系表参考下面："数据类型>> MaQueCaptureSize_e"。
*/
MaQueCaptureSize_e LibXmMaQue_Video_CaptureSize_toEnum(XM_CHAR *pCapSizeName);

/*
【功能描述】
	图像分辨率枚举值转换成名称字符串。例如：MAQUE_CAPTURE_SIZE_3M输入，将返回"3M"。
【参数】
	  参数名称			描述
	eCapSize [in]	图像分辨率枚举值。
【返回值】
	名称字符串	图像分辨率枚举值对应的字符串。
【注意】
	图像分辨率枚举值及其对应的名称字符串关系表参考下面："数据类型>> MaQueCaptureSize_e"。
*/
XM_CHAR *LibXmMaQue_Video_CaptureSize_toStr(MaQueCaptureSize_e eCapSize);

/*
【功能描述】
	视频压缩标准名称转换成枚举值。例如："H265"输入，将返回枚举值MAQUE_VIDEO_COMPRESS_H265。
【参数】
	  参数名称				描述
	pEncTypName [in]	视频压缩标准名称字符串。
【返回值】
	MaQueVideoCompress_e	视频压缩标准枚举值。不是合法字符串，将返回MAQUE_VIDEO_COMPRESS_NR。
【注意】
	视频压缩标准枚举值及其对应的名称字符串关系表参考下面："数据类型>> MaQueVideoCompress_e"。
*/
MaQueVideoCompress_e LibXmMaQue_Video_Compress_toEnum(XM_CHAR *pEncTypName);

/*
【功能描述】
	视频压缩标准枚举值转换成名称字符串。例如：MAQUE_VIDEO_COMPRESS_H265输入，将返回"H265"。
【参数】
	  参数名称				描述	
	eVideoComp [in]		视频压缩标准枚举值。
【返回值】
	名称字符串	视频压缩标准枚举值对应的字符串。
【注意】
	视频压缩标准枚举值及其对应的名称字符串关系表参考下面："数据类型>> MaQueVideoCompress_e"。
*/
XM_CHAR *LibXmMaQue_Video_Compress_toStr(MaQueVideoCompress_e eVideoComp);

/*
【功能描述】
	码率控制类型名称转换成枚举值。例如："VBR"输入，将返回枚举值MAQUE_BITRATE_CTRL_VBR。
【参数】
	  参数名称				描述
	pBitCtrlName [in]	码率控制类型名称字符串。
【返回值】
	MaQueBitrateCtrl_e	码率控制类型枚举值。不是合法字符串，将返回MAQUE_BITRATE_CTRL_NR。
【注意】
	码率控制类型枚举值及对应的名称字符串关系表参考下面："数据类型>> MaQueBitrateCtrl_e"。
*/
MaQueBitrateCtrl_e LibXmMaQue_Video_BitrateCtrl_toEnum(XM_CHAR *pBitCtrlName);

/*
【功能描述】
	码率控制类型枚举值转换成名称字符串。例如：MAQUE_BITRATE_CTRL_VBR输入，将返回"VBR"。
【参数】
	  参数名称				描述
	eBitrateCtrl [in]	码率控制类型枚举值。
【返回值】
	名称字符串	码率控制类型枚举值对应的字符串。
【注意】
	码率控制类型枚举值及对应的名称字符串关系表参考下面："数据类型>> MaQueBitrateCtrl_e"。
*/
XM_CHAR *LibXmMaQue_Video_BitrateCtrl_toStr(MaQueBitrateCtrl_e eBitrateCtrl);

/*
【功能描述】
	图像质量名称转换成枚举值。例如："Good"输入，将返回枚举值MAQUE_IMG_QUALITY_GOOD。
【参数】
	  参数名称			描述	
	pName [in]		图像质量名称字符串。
【返回值】
	MaQueImageQuality_e	图像质量枚举值。不是合法字符串，将返回MAQUE_IMG_QUALITY_NR。
【注意】
	图像质量枚举值及对应的名称字符串关系表参考下面："数据类型>> MaQueImageQuality_e"。
*/
MaQueImageQuality_e LibXmMaQue_ImageQuality_toEnum(XM_CHAR *pName);

/*
【功能描述】
	图像质量枚举值转换成名称字符串。例如：MAQUE_IMG_QUALITY_GOOD输入，将返回"Good"。
【参数】
	  参数名称				描述
	eImgQuality [in]	图像质量枚举值。
【返回值】
	名称字符串	图像质量枚举值对应的字符串。
【注意】
	图像质量枚举值及对应的名称字符串关系表参考下面："数据类型>> MaQueImageQuality_e"。
*/
XM_CHAR *LibXmMaQue_ImageQuality_toStr(MaQueImageQuality_e eImgQuality);

/*
【功能描述】
	获取底层支持的编解码最大能力。

【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。	
	pstCodeAbility	[out]	保存底层支持的编解码最大能力。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_getAbilities(XM_S32 channel, MaQueCodeAbilities_s *pstCodeAbility);

/*
【功能描述】
	强制I帧，即让编码器马上产生I帧。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		强制产生I帧的视频流通道。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_forceIFrame(XM_S32 channel, MaQueStreamChannel_e eStreamChn);

/*
【功能描述】
	动态配置视频编码器参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要修改配置的视频流通道。
	pstEncCfg [in]		视频编码参数。
【返回值】
	XM_SUCCESS		成功。
	1				需要重启。由于内存限制，一般编码格式改变（如H264和H265互切）需要重启板子。
	其它			失败，请参考错误码。
【注意】
	前面"maque_system.h"章节，LibXmMaQue_System_startUp()的参数可设定初始编码类型，调用本API时，
	只要编码类型，就可以避免重启。注意实际应用中，需要保存重启前选择的编码类型，重启后，传给
	LibXmMaQue_System_startUp()，创建需要的编码器。
*/
XM_S32 LibXmMaQue_VideoEnc_setCfg(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueVideoEncodeCfg_s *pstEncCfg);

/*
【功能描述】
	获取当前视频编码器参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要获取配置的视频流通道。
	pstEncCfg [out]		视频编码参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_getCfg(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueVideoEncodeCfg_s *pstEncCfg);

/*
【功能描述】
	注册回调函数，开始获取视频编码流。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]			需要获取视频流的通道。
	pCallbackFuncPtr [in]	获取视频编码流的回调函数指针。视频编码器编好一帧后，就通过该回调函数上传给注册者。
	pCallbackArg [in]		传入的私有结构体。在调用回调函数pCallbackFuncPtr时，传给第一个参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_startStream(XM_S32 channel, MaQueStreamChannel_e eStreamChn, 
			MaQueVideoGetStreamCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
【功能描述】
	停止获取视频编码流。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要停止视频流的通道。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_stopStream(XM_S32 channel, MaQueStreamChannel_e eStreamChn);

/*
【功能描述】
	设置视频编码等级(Profile)。H264:支持base，main，high等；而H265仅支持main。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	vencChnMask [in]	需要设置的编码通道掩码，即: 编码通道n，vencChnMask |= (1 << n)。
	eVidEncProfile [in] 需要设置的编码等级(Profile)，参考: MaQueVideoEncProfile_e。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	大部分平台(如: HI3516CV500，HI3516DV300等)的H265仅支持main，底层库写死，设置其它值，将无效。
*/

XM_S32 LibXmMaQue_VideoEnc_setProfile(XM_S32 channel, XM_U32 vencChnMask, MaQueVideoEncProfile_e eVidEncProfile);

/*
【功能描述】
	使能或关闭H264和H265 SmartP编码。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	vencChnMask [in]	需要设置的编码通道掩码，即: 编码通道n，vencChnMask |= (1 << n)。
	bEnabled [in] 		设置是否使能SmartP。1: 使能；0: 关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/

XM_S32 LibXmMaQue_VideoEnc_setSmartP(XM_S32 channel, XM_U32 vencChnMask, XM_S32 bEnabled);

/*
【功能描述】
	设置视频区域遮挡。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	index [in]		遮挡块索引，[0，3]。最多支持4个遮挡块。
	pstParam [in]	视频遮挡参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_setCover(XM_S32 channel, XM_S32 index, MaQueVideoCoverParam_s *pstParam);

/*
【功能描述】
	使能或关闭WDR模式。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bEnabled [in] 		设置是否使能WDR。1: 使能；0: 关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoEnc_switchWdrMode(XM_S32 channel, XM_S32 bEnabled);


#ifdef __cplusplus
}
#endif

#endif 


