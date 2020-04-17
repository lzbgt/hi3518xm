
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_time.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明系统时间设置和获取相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_TIME_H
#define MAQUE_TIME_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


XM_VOID LibXmMaQue_Time_print(MaQueSystemTime_s *pstSysTime, XM_CHAR *pOwer);

XM_S32 LibXmMaQue_Time_syncRtcToSystem(XM_VOID);

/*
【功能描述】
	设置系统当前时间，如果有RTC设备，同时会同步到RTC中。
【参数】
	  参数名称				描述
	pstSysTime [in]		系统当前时间。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Time_setCurrentTime(MaQueSystemTime_s *pstSysTime);

/*
【功能描述】
	获取系统当前时间。
【参数】
	  参数名称				描述
	pstSysTime [out]	保存获取的系统当前时间。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Time_getCurrentTime(MaQueSystemTime_s *pstSysTime);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

