
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_watchdog.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明看门狗操作相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_WATCH_DOG_H
#define MAQUE_WATCH_DOG_H


#ifdef __cplusplus
extern "C" {
#endif

/*
【功能描述】
	使能或停止看门狗。
【参数】
	  参数名称			描述
	bEnabled [in]	使能或停止看门狗。1：使能；0：停止。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	看门狗默认是停止，要使用看门狗，必需用本API使能。
*/
XM_S32 LibXmMaQue_Watchdog_enable(XM_S32 bEnabled);

/*
【功能描述】
	看门狗保活，即喂狗。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	看门狗使能后，默认超时时间是1分钟，要是1分钟内不调用本API喂狗，就会触发板子重启。
*/
XM_S32 LibXmMaQue_Watchdog_keepAlive(XM_VOID);

/*
【功能描述】
	设置看门狗触发重启延时时间。一旦成功调用本API，将设置看门狗超时时间为timeSecs秒，
	同时拒绝喂狗命令，也就是说timeSecs秒后，看门狗超时，导致系统重启。本API主要用于
	设置延时重启系统时间。
【参数】
	  参数名称			描述
	timeSecs [in]	看门狗触发重启延时时间，单位：秒。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Watchdog_setTimeOutSecs(XM_S32 timeSecs);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_WATCH_DOG_H


