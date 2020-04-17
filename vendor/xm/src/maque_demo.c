
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_demo.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义MaQue Demo代码主入口函数main()。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#include "demo_inc.h"

#include "demo_audio.h"
#include "demo_env.h"
#include "demo_upgrade.h"
#include "demo_jpeg_enc.h"
#include "demo_motion_detect.h"
#include "demo_mem.h"
#include "demo_net.h"
#include "demo_video_enc.h"
#include "demo_watchdog.h"
#include "demo_yuv.h"
#include "demo_rtsp.h"
#include "demo_misc.h"
#include "demo_motor.h"
#include "demo_alarm.h"
#include "demo_rtc.h"
#include "demo_video_out.h"
#include "demo_rs485.h"
#include "demo_ide.h"
#include "demo_smart.h"


typedef struct
{
	XM_S32 bEnabled;
	XM_CHAR aVidStd[8];
	XM_CHAR aVidCompMain[16];
	XM_CHAR aVidCompSecond[16];
	XM_CHAR aWritableDir[64];
} StartParamModParam_s;


XM_S32 g_bMaQueDemoDebugOn = 1;

static XM_VOID Demo_printVideoEncAbilities(MaQueCodeAbilities_s *pstCodeAbility)
{
	XM_S32 i;
	
	SIMPLE_PRINTF("videoEncTypeMask: 0x%llX.\n", pstCodeAbility->videoEncTypeMask);
	SIMPLE_PRINTF("maxEncPowerX1080P: %d.\n", pstCodeAbility->maxEncPowerX1080P);
	SIMPLE_PRINTF("eDecImageSizeMax: %d.\n", pstCodeAbility->eDecImageSizeMax);
	
	for (i = 0; i < MAQUE_STREAM_CHN_NR; i++)
	{
		SIMPLE_PRINTF("VidEncChn: %d, eCapSizeMax: %d(%s).\n", i,
			pstCodeAbility->astVidEncChnAbility[i].eCapSizeMax, 
			LibXmMaQue_Video_CaptureSize_toStr(pstCodeAbility->astVidEncChnAbility[i].eCapSizeMax));
	}
	
	return;
}

static XM_VOID Demo_usage(XM_S32 argc, XM_CHAR *argv[])
{
	fprintf(stderr, "Usage: %s [-r RefereceFile.cfg] ...\n", argv[0]);
	fprintf(stderr, "\tNotice: Default Referece File is 'MaQueReference.cfg'.\n");
	fprintf(stderr, "\t-h,-help,--help: Print Usage.\n");
	fprintf(stderr, "\tFor examples:\n");
	fprintf(stderr, "\t\t%s -r MaQueDemoJpeg.cfg\n", argv[0]);
	fprintf(stderr, "\t\t%s\n", argv[0]);
	
	return;
}

static XM_S32 Demo_processArgs(XM_S32 argc ,XM_CHAR* argv[])
{
	XM_S32 i = 0;
	
	do
	{
		if(!strcmp(argv[i], "-r"))
		{
			i++;
			MaQue_Demo_Common_setRefFile(argv[i]);
		}

		if(!strcmp(argv[i], "-h") 
			|| !strcmp(argv[i], "-help") 
			|| !strcmp(argv[i], "--help"))
		{
			Demo_usage(argc, argv);
			return XM_SUCCESS;
		}
		
		i++;

	}while(i < argc);

	return XM_SUCCESS;
}

static XM_S32 Demo_processSettings(XM_S32 argc ,XM_CHAR* argv[])
{
	XM_S32 i = 0;
	
	do
	{
		if(!strcmp(argv[i], "-e"))
		{
			i++;

			SIMPLE_PRINTF("Set Env: %s = %s.\n", argv[i], argv[i + 1]);
			
			LibXmMaQue_Env_writeKeyValue(argv[i], argv[i + 1]);
			
			i++;

			exit(0);
		}
	
		i++;

	} while(i < argc);

	return XM_SUCCESS;
}

static XM_S32 Demo_processUpgrade(XM_S32 argc ,XM_CHAR* argv[])
{
	XM_S32 res, i = 0;
	
	do
	{
		if(!strcmp(argv[i], "-u"))
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGTERM, SIG_IGN);
	
			i++;
			res = MaQue_Demo_Upgrade(argv[i]);

			while(1)
			{
				if (XM_SUCCESS == res)
				{
					INFO_PRINTF("Upgrading Is Complete, Everything Is OK; Please Reboot!\n");
				}
				else
				{
					ERROR_PRINTF("Upgrading Is Complete, Has Some Error; Please Check Log!\n");
				}
				
				sleep(1);
			}
		}
		
		i++;

	}while(i < argc);

	return XM_SUCCESS;
}

static XM_S32 Demo_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "upgrade", strlen("upgrade")))
	{
		LibXmMaQue_System_prepareForUpgrade();
		
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_getStartParamFromCfg(MaQueStartParam_s *pstStartParam)
{
	StartParamModParam_s stStartModParam = { 0 };
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &stStartModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stStartModParam.bEnabled)},
		{"VideoStd:", stStartModParam.aVidStd, XM_VALUE_TYPE_STRING, sizeof(stStartModParam.aVidStd)},
		{"MainChnVideoCompType:", stStartModParam.aVidCompMain, XM_VALUE_TYPE_STRING, sizeof(stStartModParam.aVidCompMain)},
		{"SecondChnVideoCompType:", stStartModParam.aVidCompSecond, XM_VALUE_TYPE_STRING, sizeof(stStartModParam.aVidCompSecond)},
		{"WritableDir:", stStartModParam.aWritableDir, XM_VALUE_TYPE_STRING, sizeof(stStartModParam.aWritableDir)},
	};
		
	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"StartParams", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

	if (!stStartModParam.bEnabled)
	{
		INFO_PRINTF("if (!stStartModParam.bEnabled)!\n");
		return XM_SUCCESS;
	}

	pstStartParam->eVidStd = MaQue_Demo_VidStd_str2Enum(stStartModParam.aVidStd);
	pstStartParam->aeVidComp[0] = LibXmMaQue_Video_Compress_toEnum(stStartModParam.aVidCompMain);
	pstStartParam->aeVidComp[1] = LibXmMaQue_Video_Compress_toEnum(stStartModParam.aVidCompSecond);
	strcpy(pstStartParam->aWritableDir, stStartModParam.aWritableDir);

	return XM_SUCCESS;
}

static XM_VOID Demo_handleSig(XM_S32 signo)
{
	DEBUG_PRINTF("<<Enter>>, signo: %d.!\n", signo);
	
    signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	LibXmMaQue_System_destroy();

    exit(XM_FAILURE);
}

XM_S32 main(XM_S32 argc ,XM_CHAR *argv[])
{
	XM_U64 lastTimeMs;
	MaQueMemoryApi_s stMemApi = {};
	XM_CHAR *pDebugOn;
	MaQueStartParam_s stStartParam;
	MaQueCodeAbilities_s stCodeAbility;
	ConsoleCallback_s stCallback;

	signal(SIGINT, Demo_handleSig);  /* 注册系统异常退出信号处理函数，用于销毁所有资源，便于不需要重启板子，就可再次运行Demo。*/
    signal(SIGTERM, Demo_handleSig);

	pDebugOn = getenv("MAQUE_DEMO_DEBUG_ON");  /* 运行Demo前，export MAQUE_DEMO_DEBUG_ON 1，就可开启粉色的调试打印。*/
	if (pDebugOn)
	{
		g_bMaQueDemoDebugOn = atoi(pDebugOn);
		INFO_PRINTF("g_bMaQueDemoDebugOn: %d.\n", g_bMaQueDemoDebugOn);
	}

	Demo_usage(argc, argv);

	Demo_processArgs(argc, argv);

	stMemApi.pMemAllocFuncPtr = MaQue_Demo_Mem_alloc;
	stMemApi.pMemReleaseFuncPtr = MaQue_Demo_Mem_release;
	stMemApi.pMemAddRefFuncPtr = MaQue_Demo_Mem_addRef;
	stMemApi.pMemSetLenFuncPtr = MaQue_Demo_Mem_setLength;
	
	lastTimeMs = MaQue_Demo_Common_getNowMs();

	stStartParam.eVidStd = MAQUE_VIDEO_STANDARD_PAL;
	stStartParam.aeVidComp[0] = MAQUE_VIDEO_COMPRESS_H264;
	stStartParam.aeVidComp[1] = MAQUE_VIDEO_COMPRESS_H264;
	strcpy(stStartParam.aWritableDir, "/mnt/mtd/Config/");

	Demo_getStartParamFromCfg(&stStartParam);

	LibXmMaQue_System_startUp(&stStartParam);

	INFO_PRINTF("LibXmMaQue_StartUp() Total Time:%lldms\n", MaQue_Demo_Common_getNowMs() - lastTimeMs);

	Demo_processSettings(argc, argv);
	
	Demo_processUpgrade(argc, argv);		

	LibXmMaQue_VideoEnc_getAbilities(0, &stCodeAbility);

	Demo_printVideoEncAbilities(&stCodeAbility);

    LibXmMaQue_Mem_init(&stMemApi);

	//LibXmMaQue_Gpio_setHigh(6, 7, "IRIS");

	MaQue_Demo_Net_init();

	MaQue_Demo_Env_start();

	MaQue_Demo_VideoEnc_start();
	
	MaQue_Demo_JpegEnc_start();
	
	MaQue_Demo_Audio_start();

	MaQue_Demo_Yuv_start();

	MaQue_Demo_MotionDetect_start();
	
	MaQue_Demo_Watchdog_start();

	MaQue_Demo_Upgrade_start();

	MaQue_Demo_Rtsp_start();

	//MaQue_Demo_ResetLed_start();

	MaQue_Demo_Motor_start();

	//MaQue_Demo_Alarm_start();

	//MaQue_Demo_Rtc_start();

	//MaQue_Demo_VideoOut_start();

	//MaQue_Demo_RS485_start();

	MaQue_Demo_Ide_start();

	//MaQue_Demo_Smart_start();

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
	
	while(1)
	{
		MaQue_Demo_Console_run();  /* 运行控制台任务，接收用户命令输入。收到的命令会群发给所有调用
		                            * MaQue_Demo_Console_registerCallback()注册过回调函数的线程。
		                            */
		usleep(40 * 1000);
	}

	return 0;
}


