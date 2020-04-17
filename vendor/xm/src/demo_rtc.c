
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_rtc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.05.30
 * 功能描述: 演示RTC操作相关功能。
 *
 * 修改历史:  
 ** 1.日期: 2019.05.30
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.05.30
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#include "demo_inc.h"


static XM_S32 g_bRtcStop = 0;

typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bSetTime;
	MaQueSystemTime_s stSysTime;
} RtcModParam_s;


static XM_VOID *Demo_Rtc_task(XM_VOID *pArg)
{
	MaQueSystemTime_s stSysTime = { 0 };
	RtcModParam_s stRtcModParam = { 0 };
	StringValueGet_s astValGet[] = 
	{
		{"bEnabled:", &stRtcModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.bEnabled)},
		{"bSetTime:", &stRtcModParam.bSetTime, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.bSetTime)},
		{"year:", &stRtcModParam.stSysTime.year, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.year)},
		{"month:", &stRtcModParam.stSysTime.month, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.month)},	
		{"day:", &stRtcModParam.stSysTime.day, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.day)},	
		{"hour:", &stRtcModParam.stSysTime.hour, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.hour)},
		{"minute:", &stRtcModParam.stSysTime.minute, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.minute)},
		{"second:", &stRtcModParam.stSysTime.second, XM_VALUE_TYPE_INT, sizeof(stRtcModParam.stSysTime.second)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Rtc", astValGet, sizeof(astValGet) / sizeof(StringValueGet_s));

	LibXmMaQue_Time_syncRtcToSystem();

	if (stRtcModParam.bSetTime)
	{
		LibXmMaQue_Time_setCurrentTime(&stRtcModParam.stSysTime);
	}

	while(!g_bRtcStop)
	{
		LibXmMaQue_Time_getCurrentTime(&stSysTime);
		LibXmMaQue_Time_print(&stSysTime, "Demo_Rtc_task");
	
		sleep(1);
	}


	return NULL;
}

static XM_S32 Demo_Rtc_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bRtcStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Rtc_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Rtc_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Rtc", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("RTC bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Rtc_task, NULL, "RTC");
		}
	}
	
	return XM_SUCCESS;
}


