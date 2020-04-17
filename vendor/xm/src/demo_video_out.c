
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_video_out.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示视频输出。
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

#include "demo_video_enc.h"
#include "demo_video_out.h"


typedef struct
{
	XM_S32 bEnabled;
	XM_U32 nDemoTimeSecs;
	XM_CHAR aVideoStd[8];
	XM_U32 nBindChn;
} VideoOutModParam_s;

static XM_S32 g_bVideoOutStop;

static XM_VOID Demo_VideoOut_ModParam_print(XM_CHAR *pOwner, VideoOutModParam_s *pstModParam)
{
	SIMPLE_PRINTF("@%s->Demo_VideoEnc_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstModParam->bEnabled);
	SIMPLE_PRINTF("\tbnDemoTimeSecs: %d\n", pstModParam->nDemoTimeSecs);
	SIMPLE_PRINTF("\tVideoStd: %s\n", pstModParam->aVideoStd);
	SIMPLE_PRINTF("\tnBindChn: %d\n", pstModParam->nBindChn);
}

static XM_VOID *Demo_VideoOut_task(XM_VOID *pArg)
{
	XM_S32 res;
	XM_U32 nTimeSecs = 0;
	VideoOutModParam_s stModParam = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stModParam.bEnabled)},
		{"nDemoTimeSecs:", &stModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stModParam.nDemoTimeSecs)},
		{"VideoStd:", &stModParam.aVideoStd, XM_VALUE_TYPE_STRING, sizeof(stModParam.aVideoStd)},
		{"nBindChn:", &stModParam.nBindChn, XM_VALUE_TYPE_INT, sizeof(stModParam.nBindChn)},
		
	};
	MaQueVideoOutParam_s stVoParam = { 0 };

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"VideoOut", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	Demo_VideoOut_ModParam_print("Demo_VideoOut_task()", &stModParam);

	if (!stModParam.bEnabled)
	{
		INFO_PRINTF("Video Out Module Not Enabled!\n");	
		return NULL;
	}

	if (stModParam.nDemoTimeSecs <= 0)
	{
		stModParam.nDemoTimeSecs = (XM_U32)-1;		
	}

	stVoParam.eVidStd = MaQue_Demo_VidStd_str2Enum(stModParam.aVideoStd);
	stVoParam.eVoBindChn = stModParam.nBindChn;
	stVoParam.eVoIntfType = MAQUE_VO_INTF_BT656;

	INFO_PRINTF("---------------- Video Out Start -----------------!\n");

	res = LibXmMaQue_VideoOut_create(&stVoParam);
	if (res != XM_SUCCESS)
	{
		INFO_PRINTF("LibXmMaQue_VideoOut_create() Failed!\n"); 
		return NULL;
	}
		
	
	while(!g_bVideoOutStop && nTimeSecs < stModParam.nDemoTimeSecs)
	{	
		sleep(1);
		nTimeSecs++;
	}

	LibXmMaQue_VideoOut_destroy();

	INFO_PRINTF("---------------- Video Out End, nTimeSecs:%d -----------------!\n", nTimeSecs);

	return NULL;
}

static XM_S32 Demo_VideoOut_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bVideoOutStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_VideoOut_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_VideoOut_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"VideoOut", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("VideoOut bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_VideoOut_task, NULL, "VideoOut");
		}
	}

	return XM_SUCCESS;
}


