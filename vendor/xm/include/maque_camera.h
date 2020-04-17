
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_camera.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义设置摄像头相关参数结构体，声明相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_CAMERA_H
#define MAQUE_CAMERA_H


#ifdef __cplusplus
extern "C" {
#endif


typedef XM_S32(*MaQueIrCutCallbackFuncPtr) (XM_VOID *pUserArg, XM_S32 bNightMode);
typedef XM_S32(*MaQueDayNightCallbackFuncPtr) (XM_VOID *pUserArg, XM_S32 bNightMode);

typedef enum
{
	MAQUE_WB_MODE_AUTO,  	/* 自动白平衡模式 */
	MAQUE_WB_MODE_INDOOR,  	/* 室内。手动白平衡，暂不支持。 */
	MAQUE_WB_MODE_OUTDOOR   /* 室外。手动白平衡，暂不支持。  */
} MaQueWhiteBalanceMode_e;

typedef enum
{
	MAQUE_DNC_MODE_AUTO,			// 自动切换
	MAQUE_DNC_MODE_MULTICOLOR,		// 强制为彩色
	MAQUE_DNC_MODE_BLACKWHITE,		// 强制为黑白模式
	MAQUE_DNC_MODE_BUTT
} MaQueDncMode_e;  				// 白天黑夜颜色切换模式。

typedef enum 
{
	MAQUE_DNC_THRESHOLD_LEVEL_1 = 1,	/* 白天黑夜颜色切换阈值等级1，等级越小切换越灵敏。*/
	MAQUE_DNC_THRESHOLD_LEVEL_2,    	/* 白天黑夜颜色切换阈值等级2。*/
	MAQUE_DNC_THRESHOLD_LEVEL_3,    	/* 白天黑夜颜色切换阈值等级3。*/
	MAQUE_DNC_THRESHOLD_LEVEL_4,    	/* 白天黑夜颜色切换阈值等级4。*/
	MAQUE_DNC_THRESHOLD_LEVEL_5,    	/* 白天黑夜颜色切换阈值等级5。*/
} MaQueDncThresholdLevel_e;

typedef enum
{
    MAQUE_OP_TYPE_AUTO,     /* 自动。*/
    MAQUE_OP_TYPE_MANUAL,   /* 手动。*/
    MAQUE_OP_TYPE_BUTT
} MaQueOperationType_e;

typedef enum 
{
	MAQUE_IRCUT_SWITCH_SYN_INFRARED,	/* 与红外灯同步。*/
	MAQUE_IRCUT_SWITCH_AUTO,			/* 根据环境自动。*/
} MaQueIrCutMode_e;

typedef enum 
{
	MAQUE_MANUAL_EXP_LEVEL_1 = 1,		/* 1/50 秒 */
	MAQUE_MANUAL_EXP_LEVEL_2,    		/* 1/120 秒 */
	MAQUE_MANUAL_EXP_LEVEL_3,    		/* 1/250 秒 */
	MAQUE_MANUAL_EXP_LEVEL_4,    		/* 1/500 秒 */
	MAQUE_MANUAL_EXP_LEVEL_5,    		/* 1/1000 秒 */
	MAQUE_MANUAL_EXP_LEVEL_6,    		/* 1/2000 秒 */
	MAQUE_MANUAL_EXP_LEVEL_7,    		/* 1/4000 秒 */
	MAQUE_MANUAL_EXP_LEVEL_8,    		/* 1/10000 秒 */
} MaQueManualExpLevel_e;

typedef enum 
{
	MAQUE_ES_SHUTTER_LEVEL_NONE,	/* 电子慢快门不开启。*/	
	MAQUE_ES_SHUTTER_LEVEL_1,		/* 电子慢快门等级1。帧率设置为Sensor输出帧率的4/5。 */
	MAQUE_ES_SHUTTER_LEVEL_2,    	/* 电子慢快门等级2。帧率设置为Sensor输出帧率的1/2，最大曝光时间和等级1相同。 */
	MAQUE_ES_SHUTTER_LEVEL_3,    	/* 电子慢快门等级3。帧率设置为Sensor输出帧率的1/2，最大曝光时间比等级2大。 */
	MAQUE_ES_SHUTTER_LEVEL_4,    	/* 电子慢快门等级4。帧率设置为Sensor输出帧率的1/3，最大曝光时间比上一等级大。 */
	MAQUE_ES_SHUTTER_LEVEL_5,    	/* 电子慢快门等级5。帧率设置为Sensor输出帧率的1/4，最大曝光时间比上一等级大。 */
	MAQUE_ES_SHUTTER_LEVEL_BUTT, 
} MaQueEsShutterLevel_e;

typedef enum 
{
	MAQUE_NOISE_FILTER_LEVEL_NONE,	/* 噪声滤波不开启。*/	
	MAQUE_NOISE_FILTER_LEVEL_1,		/* 噪声滤波等级1。等级越大，降噪越明显。*/
	MAQUE_NOISE_FILTER_LEVEL_2,    	/* 噪声滤波等级2。等级越大，降噪越明显。*/
	MAQUE_NOISE_FILTER_LEVEL_3,    	/* 噪声滤波等级3。等级越大，降噪越明显。*/
	MAQUE_NOISE_FILTER_LEVEL_4,    	/* 噪声滤波等级4。等级越大，降噪越明显。*/
	MAQUE_NOISE_FILTER_LEVEL_5,    	/* 噪声滤波等级5。等级越大，降噪越明显。*/
	MAQUE_NOISE_FILTER_LEVEL_BUTT, 
} MaQueNoiseFilterLevel_e;

typedef enum 
{
	MAQUE_INFRARED_DAY,
	MAQUE_INFRARED_NIGHT,
	MAQUE_INFRARED_BUTT,
} MaQueInfraredState_e;

typedef enum
{
	MAQUE_LED_TYPE_IR,  /* 红外灯。*/
	MAQUE_LED_TYPE_WL,  /* 白光灯。*/
	MAQUE_LED_TYPE_BUTT,
} MaQueLedType_e;


typedef enum
{
	MAQUE_LED_MODE_AUTO,
	MAQUE_LED_MODE_MANUAL,
	MAQUE_LED_MODE_INTELL,
	MAQUE_LED_MODE_BUTT,
} MaQueLedMode_e;

typedef struct
{
	MaQueDncMode_e eDncMode;        	 			// 日夜颜色模式，取值有彩色、自动切换和黑白
	MaQueDncThresholdLevel_e eDncThresholdLevel;	// 日夜转换阈值等级。每个等级对应不同的曝光量，实际参考该曝光量，决定是否切换日夜模式。
													// 阈值等级越小切换越灵敏。只有当eDncWorkMode为MaQue_DNC_AUTO时，才有效。
} MaQueDayNightCfg_s;

typedef struct
{
	XM_S32 nCompensationLevel;     	// 自动曝光亮度补偿等级，[0，100]，用于调节曝光的目标亮度，值越大则图像亮度越高。
	XM_U32 minTime;					// 自动曝光时间下限，[0，maxTime]，单位微秒
	XM_U32 maxTime; 				// 自动曝光时间上限，[0，0xFFFFFFFF]，单位微秒
	XM_U32 maxSysGainLevel;			// 自动曝光最大系统增益等级，[0, 100]。用于设置当前最大系统增益。100对应的系统最大
									// 增益跟sensor有关。
} MaQueAutoExpParam_s;

typedef struct
{
	MaQueManualExpLevel_e eExpLevel;	/* 手动曝光等级。设置曝光时间。*/
} MaQueManualExpParam_s;

typedef struct
{
	MaQueOperationType_e eOpType;   	/* 曝光操作类型，分: 自动和手动两种。*/
	MaQueAutoExpParam_s stAutoExp;  	/* eOpType为MAQUE_OP_TYPE_AUTO时有效。*/
	MaQueManualExpParam_s stManualExp;	/* eOpType为MAQUE_OP_TYPE_MANUAL时有效。*/
} MaQueExposureCfg_s;

typedef struct
{
	XM_U8	brightness;		///< 亮度，取值0-100。
	XM_U8	contrast;		///< 对比度，取值0-100。
	XM_U8 	saturation;		///< 饱和度，取值0-100。
	XM_U8 	hue;			///< 色调，取值0-100。
	XM_U8 	gain;			///< 增益，取值0-100。bit7置位表示自动增益，其他位被忽略。
	XM_U8	whiteBalance;	///< 自动白电平控制，bit7置位表示开启自动控制.0x0,0x1,0x2分别代表低,中,高等级
	XM_U16	acutance;       	///< 锐度，取值0-15, 第8位表示水平锐度，高8为表示垂直锐度。
} MaQueColorCfg_s;


typedef struct 
{
	XM_U32 bOpen;				/* 灯状态: 0: Close, 1: Open */
	MaQueLedType_e eLedType;	/* 灯类型: 0x0: IR; 0x1: WL */
	MaQueLedMode_e eLedMode;	/* 灯模式: 0: Auto, 1: Manual */
} MaQueLedCfg_s;

/*
【功能描述】
	通过调试串口打印输出设置给Camera的参数。主要用于调试。
【参数】
	  参数名称			描述
	pOwner	[in]	调用者标识名，用于调试。
【返回值】
	无。
【注意】
	本API主要用于开发阶段调试。
*/
XM_VOID LibXmMaQue_Camera_printParam(XM_CHAR *pOwner);


/*
【功能描述】
	注册获取IrCut切换模式的回调函数。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pCallbackFuncPtr [in]	获取当前IrCut切换模式的回调函数。
	pCallbackArg [in]		传入的注册者私有结构体。将会在调用回调函数时，传给第一个参数。只有注册者使用，
							底层不关心具体内容，因此可为NULL。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	IrCut有白天和黑夜两种滤光片，切换时会通过本API注册的回调返回是否切换到了黑夜滤光片。
*/
XM_S32 LibXmMaQue_Camera_registerIrCutCallback(XM_S32 channel, MaQueIrCutCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);	

/*
【功能描述】
	注册白天黑夜模式切换回调。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pCallbackFuncPtr [in]	获取白天和黑夜切换模式的回调函数。
	pCallbackArg [in]		传入的注册者私有结构体。将会在调用回调函数时，传给第一个参数。只有注册者使用，
							底层不关心具体内容，因此可为NULL。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	摄像头在白天和黑夜会采用不同的IrCut滤光片，也会关闭或开启红外灯或白光灯，切换时会通过本API注册的
	回调返回是否切换到黑夜模式。
*/
XM_S32 LibXmMaQue_Camera_registerDayNightCallback(XM_S32 channel, MaQueDayNightCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
【功能描述】
	设置白平衡模式。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eWBMode	[in]	白平衡模式：自动或手动白平衡。目前暂时只支持自动。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setWhiteBalanceMode(XM_S32 channel, MaQueWhiteBalanceMode_e eWBMode);

/*
【功能描述】
	获取当前白平衡模式。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	MaQueWhiteBalanceMode_e	白平衡模式，有：自动，室内，室外等。
*/
MaQueWhiteBalanceMode_e LibXmMaQue_Camera_getWhiteBalanceMode(XM_S32 channel);

/*
【功能描述】
	设置白天黑夜模式切换配置。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstDncCfg [in]	白天和黑夜切换配置参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setDayNightCfg(XM_S32 channel, MaQueDayNightCfg_s *pstDncCfg);

/*
【功能描述】
	获取白天黑夜模式切换配置。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstDncCfg [out]		保存所得的白天和黑夜切换配置参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_getDayNightCfg(XM_S32 channel, MaQueDayNightCfg_s *pstDncCfg);

/*
【功能描述】
	设置曝光配置参数。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstExpCfg [in]	曝光配置参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setExposureCfg(XM_S32 channel, MaQueExposureCfg_s *pstExpCfg);

/*
【功能描述】
	获取曝光配置参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstExpCfg [out]		保存获取的曝光配置参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_getExposureCfg(XM_S32 channel, MaQueExposureCfg_s *pstExpCfg);

/*
【功能描述】
	设置是否开启抗闪功能。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bEnabled [in]	是否开启抗闪功能。1：使能；0：关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setAntiFlicker(XM_S32 channel, XM_S32 bEnabled);

/*
【功能描述】
	查询是否开启抗闪功能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	1	抗闪使能。
	0	抗闪关闭。
*/
XM_S32 LibXmMaQue_Camera_getAntiFlicker(XM_S32 channel);

/*
【功能描述】
	设置电子慢快门等级。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eEsShutterLevel		电子慢快门等级。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setEsShutterLevel(XM_S32 channel, MaQueEsShutterLevel_e eEsShutterLevel);

/*
【功能描述】
	获取电子慢快门等级。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	MaQueEsShutterLevel_e	电子慢快门等级。
*/
MaQueEsShutterLevel_e LibXmMaQue_Camera_getEsShutterLevel(XM_S32 channel);

/*
【功能描述】
	使能或关闭背光补偿（BLC）。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bEnabled [in]	是否使能背光补偿功能。1：使能；0：关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	开启背光补偿，会使暗区变亮，清晰度提高。在宽动态模式下，背光补偿设置无效。
*/
XM_S32 LibXmMaQue_Camera_enableBLC(XM_S32 channel, XM_S32 bEnabled);

/*
【功能描述】
	查询背光补偿功能是否开启。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	1		背光补偿使能。
	0		背光补偿关闭。
*/
XM_S32 LibXmMaQue_Camera_isBLCEnabled(XM_S32 channel);

/*
【功能描述】
	设置图像左右镜像。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bMirror	[in]	是否使能左右镜像。1：使能；0：关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setMirror(XM_S32 channel, XM_S32 bMirror);

/*
【功能描述】
	查询是否左右镜像使能。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	1		左右镜像开启。
	0		左右镜像关闭。
*/
XM_S32 LibXmMaQue_Camera_getMirror(XM_S32 channel);

/*
【功能描述】
	设置图像上下翻转。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bFlip  [in]		是否使能上下翻转功能。1：使能；0：关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setFlip(XM_S32 channel, XM_S32 bFlip);

/*
【功能描述】
	查询是否上下翻转使能。
【参数】
	  参数名称			描述
	channel [in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	1		上下翻转使能。
	0		上下翻转关闭。
*/
XM_S32 LibXmMaQue_Camera_getFlip(XM_S32 channel);

/*
【功能描述】
	设置IrCut切换工作模式。
【参数】
	  参数名称				描述
	channel	[in] 		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eIrCutMode [in]		IrCut切换工作模式，有：与红外灯同步或自动。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	IrCut切换模式，只有通过前面API：LibXmMaQue_Camera_setDayNightCfg()把eDncMode设置为
	MAQUE_DNC_MODE_AUTO（自动）时才有效。如果设置为MaQue_DNC_MODE_MULTICOLOR（彩色）和
	MaQue_DNC_MODE_BLACKWHITE（黑白）模式，IrCut将会强制切换成满足相应颜色的模式。
*/
XM_S32 LibXmMaQue_Camera_setIrCutMode(XM_S32 channel, MaQueIrCutMode_e eIrCutMode);

/*
【功能描述】
	获取IrCut切换工作模式。
【参数】
	  参数名称			描述	
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	MaQueIrCutMode_e	IrCut切换工作模式，有：与红外灯同步、自动等。
*/
MaQueIrCutMode_e LibXmMaQue_Camera_getIrCutMode(XM_S32 channel);

/*
【功能描述】
	设置IrCut反序。IrCut有白天和黑夜滤光片，不同型号可能切换方式不同，又无法通过软件简单判断。
	为了兼容，特设计了本Api，可以通过软件设置IrCut反序。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	bReversed [in]	设置IrCut是否反序。1：反序；0：正常序。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_reverseIrCut(XM_S32 channel, XM_S32 bReversed);

/*
【功能描述】
	查询IrCut是否反序。
【参数】
	参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	1		IrCut反序。
	0		正常序。
*/
XM_S32 LibXmMaQue_Camera_isIrCutReversed(XM_S32 channel);

/*
【功能描述】
	获取图像的颜色参数。
【参数】
	参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstColorCfg	[out] 图像的颜色参数，参考MaQueColorCfg_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_getColor(XM_S32 channel, MaQueColorCfg_s *pstColorCfg);

/*
【功能描述】
	设置图像的颜色参数。
【参数】
	参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstColorCfg	[in] 图像的颜色参数，参考MaQueColorCfg_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setColor(XM_S32 channel, MaQueColorCfg_s *pstColorCfg);

/*
【功能描述】
	获取红外灯板光敏状态。
【参数】
	参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	光敏状态值，参考MaQueInfraredState_e 。
*/
MaQueInfraredState_e LibXmMaQue_Camera_getInfrared(XM_S32 channel);

/*
【功能描述】
	控制灯板红外灯和白光灯。
【参数】
	参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstLedCfg	[in] 灯的控制参数，参考MaQueLedCfg_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Camera_setLed(XM_S32 channel, MaQueLedCfg_s *pstLedCfg);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_CAMERA_H


