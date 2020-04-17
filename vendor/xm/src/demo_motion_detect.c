
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_motion_detect.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示移动侦测和视频遮挡功能。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#include "demo_inc.h"
#include "demo_motion_detect.h"

typedef struct
{
	MaQueMdParam_s stMdParam;
} MdModParam_s;

typedef struct
{
	MaQueBdParam_s stBdParam;;
} BdModParam_s;

typedef struct
{
	XM_S32 bMdEnabled;
	XM_S32 bBdEnabled;;
} MdBdMgr_s;

static MdBdMgr_s g_stMdBdMgr;


static XM_VOID Demo_MotionDetect_ModParam_print(XM_CHAR *pOwner, MdModParam_s *pstModParam)
{
	SIMPLE_PRINTF("@%s->Demo_JpegEnc_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstModParam->stMdParam.bEnabled);
	SIMPLE_PRINTF("\tx: %d\n", pstModParam->stMdParam.x);
	SIMPLE_PRINTF("\ty: %d\n", pstModParam->stMdParam.y);
	SIMPLE_PRINTF("\tw: %d\n", pstModParam->stMdParam.w);
	SIMPLE_PRINTF("\th: %d\n", pstModParam->stMdParam.h);
	SIMPLE_PRINTF("\teMdAlarmlevel: %d\n", pstModParam->stMdParam.eMdAlarmlevel);
}

static XM_VOID Demo_BlindDetect_ModParam_print(XM_CHAR *pOwner, BdModParam_s *pstModParam)
{
	SIMPLE_PRINTF("@%s->Demo_JpegEnc_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstModParam->stBdParam.bEnabled);
	SIMPLE_PRINTF("\teBdAlarmLevel: %d\n", pstModParam->stBdParam.eBdAlarmLevel);
}

static XM_S32 Demo_MotionDetect_callback(XM_VOID *pUserArg, MaQueMdAlarm_s *pstMdAlarm)
{
	INFO_PRINTF("Detected, eAlarmType: %d, alarmState: %d.\n", pstMdAlarm->eAlarmType, pstMdAlarm->alarmState);	

	return 0;
}

static XM_S32 Demo_BlindDetect_callback(XM_VOID *pUserArg, MaQueBdResult_s *pstBdRes)
{
	if(pstBdRes->state == 1)
	{
		INFO_PRINTF("The Camera is blind\n");	
	}
	
	return 0;
}

static XM_S32 Demo_MdBd_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	INFO_PRINTF("g_stMdBdMgr.bMdEnabled: %d, g_stMdBdMgr.bBdEnabled: %d\n", g_stMdBdMgr.bMdEnabled, g_stMdBdMgr.bBdEnabled);	
	
	if (g_stMdBdMgr.bMdEnabled)
	{
		LibXmMaQue_MD_destroy(0);
	}

	if (g_stMdBdMgr.bBdEnabled)
	{
		LibXmMaQue_BD_destroy(0);
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_MotionDetect_start(XM_VOID)
{
	ConsoleCallback_s stCallback;
	MdModParam_s stMdModParam = { 0 };
	StringValueGet_s astMdValGet[] = 
	{
		{"bEnabled:", &stMdModParam.stMdParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.bEnabled)},
		{"RegionX:", &stMdModParam.stMdParam.x, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.x)},	
		{"RegionY:", &stMdModParam.stMdParam.y, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.y)},	
		{"RegionW:", &stMdModParam.stMdParam.w, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.w)},
		{"RegionH:", &stMdModParam.stMdParam.h, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.h)},
		{"Alarmlevel:", &stMdModParam.stMdParam.eMdAlarmlevel, XM_VALUE_TYPE_INT, sizeof(stMdModParam.stMdParam.eMdAlarmlevel)},
	};
	BdModParam_s stBdModParam = { 0 };
	StringValueGet_s astBdValGet[] = 
	{
		{"bEnabled:", &stBdModParam.stBdParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stBdModParam.stBdParam.bEnabled)},
		{"eAlarmLevel:", &stBdModParam.stBdParam.eBdAlarmLevel, XM_VALUE_TYPE_INT, sizeof(stBdModParam.stBdParam.eBdAlarmLevel)},	
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"MotionDetect", astMdValGet, sizeof(astMdValGet) / sizeof(StringValueGet_s));

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"BlindDetect", astBdValGet, sizeof(astBdValGet) / sizeof(StringValueGet_s));

	Demo_MotionDetect_ModParam_print("MotionDetect", &stMdModParam);
	Demo_BlindDetect_ModParam_print("BlindDetect", &stBdModParam);

	if (stMdModParam.stMdParam.bEnabled || stBdModParam.stBdParam.bEnabled)
	{
		stCallback.pCallbackArg = NULL;
		stCallback.pCallbackFuncPtr = Demo_MdBd_Console_callback;

		MaQue_Demo_Console_registerCallback(&stCallback);
	}

	if (stMdModParam.stMdParam.bEnabled)
    {
    	LibXmMaQue_MD_create(0);

		LibXmMaQue_MD_setParam(0, &stMdModParam.stMdParam);

    	LibXmMaQue_MDAlarm_register(0, Demo_MotionDetect_callback, NULL);

		g_stMdBdMgr.bMdEnabled = 1;
	}

	if (stBdModParam.stBdParam.bEnabled)
	{
		LibXmMaQue_BD_create(0);
		
		LibXmMaQue_BD_setParam(0, &stBdModParam.stBdParam);

		LibXmMaQue_BD_register(0, Demo_BlindDetect_callback, NULL);

		g_stMdBdMgr.bBdEnabled = 1;
	}

	return 0;
}


