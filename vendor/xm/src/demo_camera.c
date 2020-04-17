
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_camera.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示Camera相关接口。
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
#include "demo_jpeg_enc.h"
#include "demo_mem.h"
#include "demo_camera.h"


static XM_S32 g_bCameraDemoStop;

static EnumStr_s g_astDncModes[MAQUE_DNC_MODE_BUTT] =
{
	{MAQUE_DNC_MODE_AUTO, "Auto"},
	{MAQUE_DNC_MODE_MULTICOLOR, "Color"},
	{MAQUE_DNC_MODE_BLACKWHITE, "Blackwhite"},
};

static EnumStr_s g_astInfraredState[MAQUE_INFRARED_BUTT] =
{
	{MAQUE_INFRARED_DAY, "Day"},
	{MAQUE_INFRARED_NIGHT, "Night"},
};

MaQueDncMode_e MaQue_Demo_DncMode_str2Enum(XM_CHAR *pName)
{
	return MaQue_Demo_String_toEnum(pName, g_astDncModes, MAQUE_DNC_MODE_BUTT);
}

XM_CHAR *MaQue_Demo_InfraredState_enum2Str(XM_U32 enumValue)
{
	return MaQue_Demo_String_enum2Str(enumValue, g_astInfraredState, MAQUE_INFRARED_BUTT);
}

XM_VOID MaQue_Demo_Camera_ModParam_print(XM_CHAR *pOwner, CameraModParam_s *pstModParam)
{
	SIMPLE_PRINTF("@%s->MaQue_Demo_Camera_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstModParam->bEnabled);
	SIMPLE_PRINTF("\tnDemoTimeSecs: %d\n", pstModParam->nDemoTimeSecs);
	SIMPLE_PRINTF("\taDncMode: %s\n", pstModParam->aDncMode);
}

static XM_S32 Demo_Camera_IrCut_callback (XM_VOID *pUserArg, XM_S32 bNightMode)
{
	INFO_PRINTF("bNightMode: %d.\n", bNightMode);
	return 0;
}

static XM_S32 Demo_Camera_DNC_callback (XM_VOID *pUserArg, XM_S32 bNightMode)
{
	INFO_PRINTF("bNightMode: %d.\n", bNightMode);
	return 0;
}

static XM_VOID *Demo_Camera_task(XM_VOID *pArg)
{
	XM_S32 nTimeSecs = 0;
	MaQueDayNightCfg_s stDncCfg;
	CameraModParam_s stCameraModParam = { 0 };
	StringValueGet_s astCamGetValGet[] = 
	{
		{"bEnabled:", &stCameraModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stCameraModParam.bEnabled)},
		{"nDemoTimeSecs:", &stCameraModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stCameraModParam.nDemoTimeSecs)},	
		{"DncMode:", &stCameraModParam.aDncMode, XM_VALUE_TYPE_STRING, sizeof(stCameraModParam.aDncMode)},	
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Camera", astCamGetValGet, sizeof(astCamGetValGet) / sizeof(StringValueGet_s));

	if (!stCameraModParam.bEnabled)
	{
		INFO_PRINTF("Camera Module Not Enabled!\n");	
		return NULL;
	}

	MaQue_Demo_Camera_ModParam_print("Demo_Camera_task", &stCameraModParam);
			
	LibXmMaQue_Camera_registerIrCutCallback(0, Demo_Camera_IrCut_callback, NULL);
	LibXmMaQue_Camera_registerDayNightCallback(0, Demo_Camera_DNC_callback, NULL);

	if (stCameraModParam.nDemoTimeSecs <= 0)
	{
		stCameraModParam.nDemoTimeSecs = (XM_U32)-1;
	}

	INFO_PRINTF("---------------- Camera Demo Start -----------------!\n");	

	LibXmMaQue_Camera_getDayNightCfg(0, &stDncCfg);
		
	stDncCfg.eDncMode = MaQue_Demo_DncMode_str2Enum(stCameraModParam.aDncMode);
		
	LibXmMaQue_Camera_setDayNightCfg(0, &stDncCfg);

	while(!g_bCameraDemoStop && nTimeSecs <= stCameraModParam.nDemoTimeSecs)
	{
		sleep(1);
		
		nTimeSecs++;
	}
	
	LibXmMaQue_Camera_registerIrCutCallback(0, NULL, NULL);
	LibXmMaQue_Camera_registerDayNightCallback(0, NULL, NULL);

	INFO_PRINTF("---------------- Camera Demo End, nTimeSecs: %d-----------------!\n", nTimeSecs);

	return NULL;
}

static XM_S32 Demo_Camera_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	MaQueEsShutterLevel_e eEsShutterLevel = MAQUE_ES_SHUTTER_LEVEL_NONE;
	XM_S32 bEnabled = 0;
	MaQueIrCutMode_e eIrCutMode = MAQUE_IRCUT_SWITCH_SYN_INFRARED;

	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bCameraDemoStop = 1;
		return XM_SUCCESS;	
	}

	if (!strncasecmp(pData, "print", strlen("print")))
	{
		LibXmMaQue_Camera_printParam("Demo_Camera_Console_callback()");
		return XM_SUCCESS;	
	}

	if (!strncasecmp(pData, "eEsShutterLevel=", strlen("eEsShutterLevel=")))
	{
		eEsShutterLevel = atoi(pData + strlen("eEsShutterLevel="));
		LibXmMaQue_Camera_setEsShutterLevel(0, eEsShutterLevel);
		return XM_SUCCESS;	
	}

	if (!strncasecmp(pData, "bBLCEnabled=", strlen("bBLCEnabled=")))
	{
		bEnabled = atoi(pData + strlen("bBLCEnabled="));
		LibXmMaQue_Camera_enableBLC(0, bEnabled);
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "bMirror=", strlen("bMirror=")))
	{
		bEnabled = atoi(pData + strlen("bMirror="));
		LibXmMaQue_Camera_setMirror(0, bEnabled);
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "bFlip=", strlen("bFlip=")))
	{
		bEnabled = atoi(pData + strlen("bFlip="));
		LibXmMaQue_Camera_setFlip(0, bEnabled);
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "eDncMode=", strlen("eDncMode=")))
	{
		MaQueDayNightCfg_s stDncCfg;
		MaQueDncMode_e eDncMode;

		LibXmMaQue_Camera_getDayNightCfg(0, &stDncCfg);
		
		eDncMode = atoi(pData + strlen("eDncMode="));

		stDncCfg.eDncMode = eDncMode;
		
		LibXmMaQue_Camera_setDayNightCfg(0, &stDncCfg);
		
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "eIrCutMode=", strlen("eIrCutMode=")))
	{
		eIrCutMode = atoi(pData + strlen("eIrCutMode="));
		LibXmMaQue_Camera_setIrCutMode(0, eIrCutMode);
		return XM_FAILURE;	
	}
	
	if (!strncasecmp(pData, "bIrCutReversed=", strlen("bIrCutReversed=")))
	{
		bEnabled = atoi(pData + strlen("bIrCutReversed="));
		LibXmMaQue_Camera_reverseIrCut(0, bEnabled);
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "eExpOpType=", strlen("eExpOpType=")))
	{
		MaQueExposureCfg_s stExpCfg;
		MaQueOperationType_e eOpType;
		
		LibXmMaQue_Camera_getExposureCfg(0, &stExpCfg);
		
		eOpType = atoi(pData + strlen("eExpOpType="));

		stExpCfg.eOpType = eOpType;
		
		LibXmMaQue_Camera_setExposureCfg(0, &stExpCfg);
		
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "eExpCompLevel=", strlen("eExpCompLevel=")))
	{
		MaQueExposureCfg_s stExpCfg;
		XM_S32 nCompensationLevel;
		
		LibXmMaQue_Camera_getExposureCfg(0, &stExpCfg);
		
		nCompensationLevel = atoi(pData + strlen("eExpCompLevel="));

		stExpCfg.stAutoExp.nCompensationLevel = nCompensationLevel;
		
		LibXmMaQue_Camera_setExposureCfg(0, &stExpCfg);
		
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "MaxExpSysGainLevel=", strlen("MaxExpSysGainLevel=")))
	{
		MaQueExposureCfg_s stExpCfg;
		XM_S32 nCompensationLevel;
		
		LibXmMaQue_Camera_getExposureCfg(0, &stExpCfg);
		
		nCompensationLevel = atoi(pData + strlen("MaxExpSysGainLevel="));

		stExpCfg.stAutoExp.nCompensationLevel = nCompensationLevel;
		
		LibXmMaQue_Camera_setExposureCfg(0, &stExpCfg);
		
		return XM_FAILURE;	
	}

	if (!strncasecmp(pData, "stColorCfgSaturation=", strlen("stColorCfgSaturation=")))
	{
		MaQueColorCfg_s stColorCfg;
		XM_U8 saturation;
		
		LibXmMaQue_Camera_getColor(0, &stColorCfg);
		
		saturation = atoi(pData + strlen("stColorCfgSaturation="));

		stColorCfg.saturation = saturation;
		
		LibXmMaQue_Camera_setColor(0, &stColorCfg);
		
		return XM_FAILURE;
	}

	if (!strncasecmp(pData, "getInfrared", strlen("getInfrared")))
	{
		XM_S32 state;
		state = LibXmMaQue_Camera_getInfrared(0);

		INFO_PRINTF("LibXmMaQue_Camera_getInfrared : %s\n", MaQue_Demo_InfraredState_enum2Str(state));
		
		return XM_FAILURE;
	}

	if (!strncasecmp(pData, "stLedCfgIrbOpen=", strlen("stLedCfgIrbOpen=")))
	{
		MaQueLedCfg_s stLedCfg;
		XM_S32 bOpen;
		
		bOpen = atoi(pData + strlen("stLedCfgIrbOpen="));

		stLedCfg.bOpen = bOpen;
		stLedCfg.eLedMode = MAQUE_LED_MODE_AUTO;
		stLedCfg.eLedType = MAQUE_LED_TYPE_IR;

		LibXmMaQue_Camera_setLed(0, &stLedCfg);
		
		return XM_FAILURE;
	}

	if (!strncasecmp(pData, "stLedCfgWlbOpen=", strlen("stLedCfgWlbOpen=")))
	{
		MaQueLedCfg_s stLedCfg;
		XM_S32 bOpen;
		
		bOpen = atoi(pData + strlen("stLedCfgWlbOpen="));

		stLedCfg.bOpen = bOpen;
		stLedCfg.eLedMode = MAQUE_LED_MODE_AUTO;
		stLedCfg.eLedType = MAQUE_LED_TYPE_WL;

		LibXmMaQue_Camera_setLed(0, &stLedCfg);
		
		return XM_FAILURE;
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Camera_start(XM_VOID)
{
	XM_S32 bEnabled = 0, nEnabledCount = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Camera_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"Camera", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

	if (bEnabled)
	{
		MaQue_Demo_Common_createThread(Demo_Camera_task, (XM_VOID *)1, "Camera");
		nEnabledCount++;
	}

	return 0;
}

