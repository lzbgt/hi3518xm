
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_motion_detect.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义移动侦测和视频遮挡相关结构体，声明相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_MD_H
#define MAQUE_MD_H


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	MAQUE_MD_ALARM_TYPE = 7,
} MaQueMdAlarmType_e;

typedef struct 
{
	MaQueMdAlarmType_e eAlarmType;  /* 移动侦测报警类型。 */
	XM_S32 alarmState;   /* 报警状态值。1：有报警；0：无报警。*/
	XM_CHAR *pAlarmFileName;   /* 保留。*/
} MaQueMdAlarm_s;

typedef XM_S32 (*MaQueMdAlarmCallbackFuncPtr)(XM_VOID *pUserArg, MaQueMdAlarm_s *pstMdAlarm);

typedef enum
{
	MAQUE_MD_ALARM_LEVEL_1 = 1,
	MAQUE_MD_ALARM_LEVEL_2,
	MAQUE_MD_ALARM_LEVEL_3,
	MAQUE_MD_ALARM_LEVEL_4,
	MAQUE_MD_ALARM_LEVEL_5,
	MAQUE_MD_ALARM_LEVEL_6,
	MAQUE_MD_ALARM_LEVEL_MAX,
} MaQueMdAlarmLevel_e;

typedef enum
{
	MAQUE_BD_ALARM_LEVEL_1 = 1,
	MAQUE_BD_ALARM_LEVEL_2,
	MAQUE_BD_ALARM_LEVEL_3,
	MAQUE_BD_ALARM_LEVEL_4,
	MAQUE_BD_ALARM_LEVEL_5,
	MAQUE_BD_ALARM_LEVEL_6,
	MAQUE_BD_ALARM_LEVEL_MAX,
} MaQueBdAlarmLevel_e;


typedef struct
{
	XM_S32 bEnabled;   /* 移动侦测使能开关。1：使能；0：关闭。*/
	XM_U32 x;   /* 移动侦测区域起始横坐标。相对CIF（352 * 288）坐标系。*/
	XM_U32 y;   /* 移动侦测区域起始纵坐标。相对CIF（352 * 288）坐标系。*/
	XM_U32 w;   /* 移动侦测区域宽。相对CIF（352 * 288）坐标系。*/
	XM_U32 h;   /* 移动侦测区域高。相对CIF（352 * 288）坐标系。*/
	MaQueMdAlarmLevel_e eMdAlarmlevel;			/* 移动侦测灵敏度等级。值越大越灵敏。*/
} MaQueMdParam_s;

typedef struct
{
	XM_S32 bEnabled;  /* 视频遮挡使能开关。1：使能；0：关闭。 */
	MaQueBdAlarmLevel_e eBdAlarmLevel;  /* 视频遮挡灵敏度等级。值越大越灵敏。 */
}MaQueBdParam_s;


typedef struct
{
	XM_S32 state;   /* 视频遮挡状态。1：有遮挡；0：无遮挡。 */
} MaQueBdResult_s;

typedef XM_S32(*MaQueBdGetResultFuncPtr)(XM_VOID *pUserArg, MaQueBdResult_s *pstBdRes);

/*
【功能描述】
	创建移动侦测任务。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MD_create(XM_S32 channel);

/*
【功能描述】
	设置移动侦测参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstMdParam [in]		需设置的移动侦测参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MD_setParam(XM_S32 channel, MaQueMdParam_s *pstMdParam);
/*
【功能描述】
	获取移动侦测参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstMdParam [out]	保存所获取的移动侦测参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MD_getParam(XM_S32 channel, MaQueMdParam_s *pstMdParam);

/*
【功能描述】
	注册获取移动侦测报警回调函数。
【参数】
	  参数名称					描述
	channel	 [in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pCallbackFuncPtr [in]	获取移动侦测报警的回调函数指针。
	pCallbackArg [in]		注册者传入的私有参数，将传给pCallbackFuncPtr的第一个参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MDAlarm_register(XM_S32 channel, MaQueMdAlarmCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);
/*
【功能描述】
	销毁移动侦测任务。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_MD_destroy(XM_S32 channel);

/*
【功能描述】
	创建视频遮挡检测任务。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_BD_create(XM_S32 channel);

/*
【功能描述】
	设置视频遮挡参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstBdParam [in]		需设置的视频遮挡参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_BD_setParam(XM_S32 channel, MaQueBdParam_s *pstBdParam);

/*
【功能描述】
	获取视频遮挡参数。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pstBdParam	[out]	需设置的视频遮挡参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_BD_getParam(XM_S32 channel, MaQueBdParam_s *pstBdParam);

/*
【功能描述】
	注册获取视频遮挡结果的回调函数。
【参数】
	  参数名称					描述
	channel	[in]			对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	pCallbackFuncPtr [in]	获取视频遮挡结果的回调函数指针。
	pCallbackArg [in]		注册者传入的私有参数，将传给pCallbackFuncPtr的第一个参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_BD_register(XM_S32 channel, MaQueBdGetResultFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
【功能描述】
	创建视频遮挡检测任务。
【参数】
	  参数名称			描述
	channel	[in]	对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_BD_destroy(XM_S32 channel);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MD_H


