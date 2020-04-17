
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_rtsp.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示Rtsp实时流媒体播放。
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
#include "demo_mem.h"
#include "demo_audio.h"
#include "demo_video_enc.h"
#include "demo_rtsp.h"
#include "libxmrtsp_api.h"
#include "demo_camera.h"


typedef struct
{
	rtsp_handle rtspHandle;
	XM_S32 bAudio;
	MaQueStreamChannel_e eStreamChn;
	XM_S32 bClosePrint;
} RtspCallbackArg_s;

static XM_S32 g_bRtspStop;

static XM_S32 Demo_Rtsp_Audio_record_callback(XM_VOID *pUserArg, MaQueAudioFrameInfo_s *pstFrameInfo)
{
	RtspCallbackArg_s *pstCallbackArg = (RtspCallbackArg_s *) pUserArg;
	MaQueTimeStamp_s *pstTimeStamp = &pstFrameInfo->stTimeStamp;
	MaQueSystemTime_s *pstSysTime = &pstTimeStamp->stSysTime;
	struct timeval stTimeVal;
	
	//INFO_PRINTF("Frame,Time: %ds + %dms\n", pstSysTime->second, pstFrameInfo->stTimeStamp.mSec);
	
	stTimeVal.tv_sec = pstSysTime->day * 24 * 60 * 60 + pstSysTime->hour * 60 * 60
			+ pstSysTime->minute * 60 + pstSysTime->second;
	stTimeVal.tv_usec = pstSysTime->second * 1000;

	rtp_send_media(pstCallbackArg->rtspHandle, 0, pstCallbackArg->eStreamChn, 
		(XM_S8 *)pstFrameInfo->pData, pstFrameInfo->nDataLen, &stTimeVal);
	
	return XM_SUCCESS;
}

static XM_S32 Demo_Rtsp_Camera_DNC_callback (XM_VOID *pUserArg, XM_S32 bNightMode)
{
	INFO_PRINTF("bNightMode: %d.\n", bNightMode);
	MaQueLedCfg_s stLedCfg;
	XM_S32 bOpen;

	if (bNightMode)
	{
		bOpen = 1;
	}
	else
	{
		bOpen = 0;
	}

	stLedCfg.bOpen = bOpen;
	stLedCfg.eLedMode = MAQUE_LED_MODE_AUTO;
	stLedCfg.eLedType = MAQUE_LED_TYPE_IR;

	LibXmMaQue_Camera_setLed(0, &stLedCfg);
	
	return XM_SUCCESS;
}

static XM_VOID *Demo_Rtsp_ResetLed_task(XM_VOID *pArg)
{
	XM_U32 value;
	MaQueLedCfg_s stLedCfg;

	INFO_PRINTF("---------------- Rtsp_ResetLed Start -----------------!\n");

	while(1)
	{
		LibXmMaQue_Misc_readResetKey(&value);
		
		if (!value)
		{
			stLedCfg.bOpen = 1;
			stLedCfg.eLedMode = MAQUE_LED_MODE_AUTO;
			stLedCfg.eLedType = MAQUE_LED_TYPE_WL;

			LibXmMaQue_Camera_setLed(0, &stLedCfg);

			sleep(3);

			stLedCfg.bOpen = 0;
			stLedCfg.eLedMode = MAQUE_LED_MODE_AUTO;
			stLedCfg.eLedType = MAQUE_LED_TYPE_WL;

			LibXmMaQue_Camera_setLed(0, &stLedCfg);
		}
		
		sleep(1);
	}

	return NULL;
}

static XM_S32 Demo_Rtsp_Audio_start(rtsp_handle rtspHandle)
{
	XM_S32 res;
	RtspCallbackArg_s stCallbackArg;
	AudioRecordModParam_s stAudModParam = { 0 };
	MaQueVqeCfg_s stMaQueVqeCfg;
	MaQueAudioEncodeFormat_s stAudEncFormat = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stAudModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bEnabled)},
		{"bWriteFile:", &stAudModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bWriteFile)},
		{"nDemoTimeSecs:", &stAudModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stAudModParam.nDemoTimeSecs)},	
	};
	
	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RtspAudioRecord", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	if (!stAudModParam.bEnabled)
	{
		INFO_PRINTF("if (!stAudModParam.bEnabled)\n");
		return XM_FAILURE;
	}
	
	INFO_PRINTF(" ------------ Audio Record Start ---------------\n");
	
	INFO_PRINTF("bWriteFile: %d, nDemoTimeSecs: %d.\n", stAudModParam.bWriteFile, stAudModParam.nDemoTimeSecs);
	
	stAudEncFormat.eEncType = MAQUE_AUDIO_ENCODE_G711_ALAW; 
	stAudEncFormat.eSampleRate = MAQUE_AUDIO_SAMPLE_RATE_8000; 
	stAudEncFormat.eSampleBits = MAQUE_AUDIO_SAMPLE_BITS_16; 
	stAudEncFormat.bitrateKb = 0;  /* G711A和G711U不需要设置比特率。*/

	stCallbackArg.bAudio = 1;
	stCallbackArg.eStreamChn = RTSP_MEDIA_DWTYPE_AUDIO;
	stCallbackArg.rtspHandle = rtspHandle;
	
	res = LibXmMaQue_AudioEnc_create(0, &stAudEncFormat, Demo_Rtsp_Audio_record_callback, &stCallbackArg);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_AudioEnc_create() Failed!\n");
		return XM_FAILURE;
	}

	LibXmMaQue_Audio_setMicVolume(1, (30 << 8) | 46);
	
	LibXmMaQue_Audio_setSpeakerVolume(1, 15);

	res = LibXmMaQue_Audio_getAiVqeAttr(0, &stMaQueVqeCfg);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_Audio_getAiVqeAttr() Failed!\n");
		return XM_FAILURE;
	}

	stMaQueVqeCfg.bVqeEnable = 1;
	stMaQueVqeCfg.bAecEnable = 0;
	
	res = LibXmMaQue_Audio_setAiVqeAttr(0, &stMaQueVqeCfg);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_Audio_getAiVqeAttr() Failed!\n");
		return XM_FAILURE;
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_Rtsp_getVideoStream_callback(XM_VOID *pUserArg, MaQueVideoEncFrameInfo_s *pstFrameInfo)
{
	RtspCallbackArg_s *pstCallbackArg = (RtspCallbackArg_s *) pUserArg;
	MaQueSystemTime_s *pstSysTime = &pstFrameInfo->stTimeStamp.stSysTime;
	struct timeval stTimeVal;

	if (!pstCallbackArg->bClosePrint)
	{
		//DEBUG_PRINTF("<%s> Frame,Time: %ds + %dms, pts: %lld <Stream#%d>\n", 
		//	0 == pstFrameInfo->eSubType ? "I" : "P", pstSysTime->second,
		//	pstFrameInfo->stTimeStamp.mSec, pstFrameInfo->pts, pstCallbackArg->eStreamChn);
		
		if (MAQUE_FRAME_SUBTYPE_I == pstFrameInfo->eSubType)
		{
			DEBUG_PRINTF("<I> Frame, Time: %ds + %dms, pts: %lld <Stream#%d>\n", 
				pstSysTime->second, pstFrameInfo->stTimeStamp.mSec, pstFrameInfo->pts, pstCallbackArg->eStreamChn);
		}
	}
	
	//INFO_PRINTF("eStreamChn = %d, eSubType = %d, nFrameRate = %d\n", pstCallbackArg->eStreamChn, pstFrameInfo->eSubType, pstFrameInfo->nFrameRate);
	
	stTimeVal.tv_sec = pstSysTime->day * 24 * 60 * 60 + pstSysTime->hour * 60 * 60
			+ pstSysTime->minute * 60 + pstSysTime->second;
	stTimeVal.tv_usec = pstSysTime->second * 1000;

	rtp_send_media(pstCallbackArg->rtspHandle, 0, pstCallbackArg->eStreamChn, 
		(XM_S8 *)pstFrameInfo->pData, pstFrameInfo->nDataLen, &stTimeVal);

	MaQue_Demo_Mem_release(pstFrameInfo->handleMem);

	return XM_SUCCESS;
}

static XM_S32 Demo_Rtsp_setCameraParam(XM_VOID)
{
	MaQueDayNightCfg_s stDncCfg;
	CameraModParam_s stCameraModParam = { 0 };
	IRWLModParam_s stIRWLModParam = { 0 };
	StringValueGet_s astCamGetValGet[] = 
	{
		{"bEnabled:", &stCameraModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stCameraModParam.bEnabled)},
		{"nDemoTimeSecs:", &stCameraModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stCameraModParam.nDemoTimeSecs)}, 
		{"DncMode:", &stCameraModParam.aDncMode, XM_VALUE_TYPE_STRING, sizeof(stCameraModParam.aDncMode)},
		{"bIREnabled:", &stIRWLModParam.bIREnabled, XM_VALUE_TYPE_INT, sizeof(stIRWLModParam.bIREnabled)},
		{"bWLEnabled:", &stIRWLModParam.bWLEnabled, XM_VALUE_TYPE_INT, sizeof(stIRWLModParam.bWLEnabled)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RtspCamera", astCamGetValGet, sizeof(astCamGetValGet) / sizeof(StringValueGet_s));

	if (!stCameraModParam.bEnabled)
	{
		INFO_PRINTF("Camera Module Not Enabled!\n");	
		return XM_SUCCESS;
	}

	MaQue_Demo_Camera_ModParam_print("Demo_Rtsp_setCameraParam", &stCameraModParam);

	LibXmMaQue_Camera_getDayNightCfg(0, &stDncCfg);
		
	stDncCfg.eDncMode = MaQue_Demo_DncMode_str2Enum(stCameraModParam.aDncMode);
		
	LibXmMaQue_Camera_setDayNightCfg(0, &stDncCfg);

	if (stIRWLModParam.bWLEnabled)
	{
		LibXmMaQue_Misc_ResetLedInit();

		MaQue_Demo_Common_createThread(Demo_Rtsp_ResetLed_task, NULL, "Rtsp_ResetLed");
	}
	
	if (stIRWLModParam.bIREnabled)
	{
		LibXmMaQue_Camera_registerDayNightCallback(0, Demo_Rtsp_Camera_DNC_callback, NULL);
	}

	return XM_SUCCESS;
}

static XM_VOID *Demo_Rtsp_task(XM_VOID *pArg)
{
	XM_S32 res;
	XM_U32 nTimeSecs = 0;
	XM_S32 bOsdInited = 0;
	RtspCallbackArg_s stCallbackArg[MAQUE_STREAM_CHN_NR];
	rtsp_handle handleRtsp;
	rtsp_media_attr_t stRtspMediaAttr = { 0 };
	VideoEncodeModParam_s stVidEncModParamMain = { 0 };
	VideoEncodeModParam_s stVidEncModParamSec = { 0 };
	VideoEncodeModParam_s stVidEncModParam = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stVidEncModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.bEnabled)},
		{"bWriteFile:", &stVidEncModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.bWriteFile)},
		{"bClosePrint:", &stVidEncModParam.bClosePrint, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.bClosePrint)},
		{"nDemoTimeSecs:", &stVidEncModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.nDemoTimeSecs)},	
		{"LogoFile:", &stVidEncModParam.aLogoFile, XM_VALUE_TYPE_STRING, sizeof(stVidEncModParam.aLogoFile)},	
		{"CompressType:", &stVidEncModParam.aVidComp, XM_VALUE_TYPE_STRING, sizeof(stVidEncModParam.aVidComp)},
		{"CaptureSize:", &stVidEncModParam.aCapSize, XM_VALUE_TYPE_STRING, sizeof(stVidEncModParam.aCapSize)},
		{"BitrateCtrlType:", &stVidEncModParam.aBitrateCtrl, XM_VALUE_TYPE_STRING, sizeof(stVidEncModParam.aBitrateCtrl)},
		{"ImageQuality:", &stVidEncModParam.aImgQuality, XM_VALUE_TYPE_STRING, sizeof(stVidEncModParam.aImgQuality)},
		{"nFps:", &stVidEncModParam.nFps, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.nFps)},
		{"nBitRate:", &stVidEncModParam.nBitRate, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.nBitRate)},
		{"nIFrameInterval:", &stVidEncModParam.nIFrameInterval, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.nIFrameInterval)},
		{"iQpMin:", &stVidEncModParam.iQpMin, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.iQpMin)},
		{"iQpMax:", &stVidEncModParam.iQpMax, XM_VALUE_TYPE_INT, sizeof(stVidEncModParam.iQpMax)},
	};
	OsdTitleModParam_s stOsdTimeModParamMain = { 0 };
	OsdTitleModParam_s stOsdTimeModParamSec = { 0 };
	OsdTitleModParam_s stOsdTitleModParamMain = { 0 };
	OsdTitleModParam_s stOsdTitleModParamSec = { 0 };
	OsdTitleModParam_s stOsdTitleModParam = { 0 };
	StringValueGet_s astOsdTitleValGet[] = 
	{
		{"bEnabled:", &stOsdTitleModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.bEnabled)},
		{"Index:", &stOsdTitleModParam.stOsdParam.index, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.index)},	
		{"Left:", &stOsdTitleModParam.stOsdParam.left, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.left)},	
		{"Top:", &stOsdTitleModParam.stOsdParam.top, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.top)},	
		{"FontSize:", &stOsdTitleModParam.stOsdParam.eFontSize, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.eFontSize)},
		{"FgTrans:", &stOsdTitleModParam.stOsdParam.fgTrans, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.fgTrans)},
		{"BgTrans:", &stOsdTitleModParam.stOsdParam.bgTrans, XM_VALUE_TYPE_INT, sizeof(stOsdTitleModParam.stOsdParam.bgTrans)},
		{"FgColor:", &stOsdTitleModParam.stOsdParam.fgColor, XM_VALUE_TYPE_HEX, sizeof(stOsdTitleModParam.stOsdParam.fgColor)},
		{"BgColor:", &stOsdTitleModParam.stOsdParam.bgColor, XM_VALUE_TYPE_HEX, sizeof(stOsdTitleModParam.stOsdParam.bgColor)},
		{"FontLibPath:", &stOsdTitleModParam.aFontLibPath, XM_VALUE_TYPE_STRING, sizeof(stOsdTitleModParam.aFontLibPath)},
		{"Title:", &stOsdTitleModParam.aTitle, XM_VALUE_TYPE_STRING, sizeof(stOsdTitleModParam.aTitle)},
	};
	XM_CHAR *pEnv = NULL;
	XM_CHAR *pFontFile = NULL;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RtspVideoMainChnEncode", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	MaQue_Demo_VideoEnc_ModParam_print("Demo_Rtsp_task() <Video Main Chn>", &stVidEncModParam);

	stVidEncModParamMain = stVidEncModParam;
	
	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RtspVideoSecondChnEncode", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	stVidEncModParamSec = stVidEncModParam;

	MaQue_Demo_VideoEnc_ModParam_print("Demo_Rtsp_task() <Video Second Chn>", &stVidEncModParam);

	if (!stVidEncModParamMain.bEnabled && !stVidEncModParamSec.bEnabled)
	{
		INFO_PRINTF("All Video Encode Module Not Enabled!\n");	
		return NULL;
	}

	Demo_Rtsp_setCameraParam();

	handleRtsp = rtsp_create(4, 5, 1); 
	if (!handleRtsp)
	{
		ERROR_PRINTF("rtsp_create err!\n");
		return NULL;
	}

	if (stVidEncModParamMain.bEnabled)
	{
		res = MaQue_Demo_VideoEnc_setCfg(MAQUE_STREAM_CHN_MAIN, &stVidEncModParamMain);
		if (res < 0)
		{
			ERROR_PRINTF("Demo_VideoEnc_setCfg() Failed, eStreamChn: %d!\n", MAQUE_STREAM_CHN_MAIN);
		}
	
		if (1 == res)
		{
			ERROR_PRINTF("Need to reboot, eStreamChn: %d!\n", MAQUE_STREAM_CHN_MAIN);
			return NULL;
		}
	}

	if (stVidEncModParamSec.bEnabled)
	{
		res = MaQue_Demo_VideoEnc_setCfg(MAQUE_STREAM_CHN_2END, &stVidEncModParamSec);
		if (res < 0)
		{
			ERROR_PRINTF("Demo_VideoEnc_setCfg() Failed, eStreamChn: %d!\n", MAQUE_STREAM_CHN_2END);
		}

		if (1 == res)
		{
			ERROR_PRINTF("Need to reboot, eStreamChn: %d!\n", MAQUE_STREAM_CHN_2END);
			return NULL;
		}
	}
	
	stRtspMediaAttr.video_type[0] = LibXmMaQue_Video_Compress_toEnum(stVidEncModParamMain.aVidComp) 
			== MAQUE_VIDEO_COMPRESS_H265 ? RTSP_PAYLOAD_TYPE_H265 : RTSP_PAYLOAD_TYPE_H264;
	stRtspMediaAttr.video_type[1] = LibXmMaQue_Video_Compress_toEnum(stVidEncModParamSec.aVidComp) 
			== MAQUE_VIDEO_COMPRESS_H265 ? RTSP_PAYLOAD_TYPE_H265 : RTSP_PAYLOAD_TYPE_H264;
	stRtspMediaAttr.video_fps[0] = stVidEncModParamMain.nFps;
	stRtspMediaAttr.video_fps[1] = stVidEncModParamSec.nFps;
	stRtspMediaAttr.audio_type = RTSP_PAYLOAD_TYPE_G711_PCMA,
	stRtspMediaAttr.audio_sample_rate = 8000,
	stRtspMediaAttr.audio_poinum = 160,

	rtsp_set_media_attr(handleRtsp, &stRtspMediaAttr);

	stCallbackArg[MAQUE_STREAM_CHN_MAIN].rtspHandle = handleRtsp;
	stCallbackArg[MAQUE_STREAM_CHN_MAIN].bClosePrint = stVidEncModParam.bClosePrint;
	
	if (stVidEncModParamMain.bEnabled)
	{
		stCallbackArg[MAQUE_STREAM_CHN_MAIN].eStreamChn = MAQUE_STREAM_CHN_MAIN;
	
		LibXmMaQue_VideoEnc_startStream(0, MAQUE_STREAM_CHN_MAIN, Demo_Rtsp_getVideoStream_callback, &stCallbackArg[MAQUE_STREAM_CHN_MAIN]);
	}

	stCallbackArg[MAQUE_STREAM_CHN_2END].rtspHandle = handleRtsp;
	stCallbackArg[MAQUE_STREAM_CHN_2END].bClosePrint = stVidEncModParam.bClosePrint;
	if (stVidEncModParamSec.bEnabled)
	{
		stCallbackArg[MAQUE_STREAM_CHN_2END].eStreamChn = MAQUE_STREAM_CHN_2END;
	
		LibXmMaQue_VideoEnc_startStream(0, MAQUE_STREAM_CHN_2END, Demo_Rtsp_getVideoStream_callback, &stCallbackArg[MAQUE_STREAM_CHN_2END]);
	}

	Demo_Rtsp_Audio_start(handleRtsp);

	if (stVidEncModParamMain.bEnabled && !access(stVidEncModParamMain.aLogoFile, F_OK))
	{
		LibXmMaQue_OSD_setLogo(0, MAQUE_STREAM_CHN_MAIN, 5, stVidEncModParamMain.aLogoFile, 2);
	}

	if (stVidEncModParamSec.bEnabled && !access(stVidEncModParamSec.aLogoFile, F_OK))
	{
		LibXmMaQue_OSD_setLogo(0, MAQUE_STREAM_CHN_2END, 5, stVidEncModParamSec.aLogoFile, 2);
	}

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"RtspOsdTimeMainChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	stOsdTimeModParamMain = stOsdTitleModParam;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"RtspOsdTimeSecChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	stOsdTimeModParamSec = stOsdTitleModParam;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"RtspOsdTitleMainChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	stOsdTitleModParamMain = stOsdTitleModParam;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"RtspOsdTitleSecChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	stOsdTitleModParamSec = stOsdTitleModParam;

	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTimeModParamMain", &stOsdTimeModParamMain);
	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTimeModParamSec", &stOsdTimeModParamSec);
	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTitleModParamMain", &stOsdTitleModParamMain);
	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTitleModParamSec", &stOsdTitleModParamSec);

	pEnv = getenv("MAQUE_FONT_FILE");
	if (pEnv)
	{
		INFO_PRINTF("pEnv: %s.\n", pEnv);
	}

	if (stVidEncModParamMain.bEnabled && stOsdTimeModParamMain.bEnabled)
	{
		LibXmMaQue_OSD_showTime(0, MAQUE_STREAM_CHN_MAIN, &stOsdTimeModParamMain.stOsdParam);

		pFontFile = stOsdTitleModParamMain.aFontLibPath;
		
		if (pEnv)
		{
			pFontFile = pEnv;
		}
			
		if (pFontFile[0] && strncasecmp(pFontFile, "NULL", strlen("NULL")))
		{
			XM_S32 i;

			INFO_PRINTF("aFontLibPath: %s <Main Chn>.\n", pFontFile);

			LibXmMaQue_OSD_init(pFontFile);
			
			for (i = 0; i < 1; i++)
			{
				LibXmMaQue_OSD_setTitle(0, MAQUE_STREAM_CHN_MAIN, &stOsdTitleModParamMain.stOsdParam, stOsdTitleModParamMain.aTitle);
				usleep(100 *1000);
			}
	
			bOsdInited = 1;
		}
	}	

	if (stVidEncModParamSec.bEnabled && stOsdTimeModParamSec.bEnabled)
	{
		LibXmMaQue_OSD_showTime(0, MAQUE_STREAM_CHN_2END, &stOsdTimeModParamSec.stOsdParam);

		pFontFile = stOsdTitleModParamSec.aFontLibPath;
		
		if (pEnv)
		{
			pFontFile = pEnv;
		}
		
		if (pFontFile[0] && strncasecmp(pFontFile, "NULL", strlen("NULL")))
		{
			XM_S32 i;

			INFO_PRINTF("aFontLibPath: %s <Sec Chn>.\n", pFontFile);

			LibXmMaQue_OSD_init(pFontFile);
			
			for (i = 0; i < 1; i++)
			{
				LibXmMaQue_OSD_setTitle(0, MAQUE_STREAM_CHN_2END, &stOsdTitleModParamSec.stOsdParam, stOsdTitleModParamSec.aTitle);
				usleep(100 *1000);
			}
	
			bOsdInited = 1;
		}
	}	

	if (stVidEncModParamMain.nDemoTimeSecs <= 0 || stVidEncModParamSec.nDemoTimeSecs <= 0)
	{
		stVidEncModParam.nDemoTimeSecs = (XM_U32)-1;		
	}
	else
	{
		stVidEncModParam.nDemoTimeSecs = stVidEncModParamMain.nDemoTimeSecs >= stVidEncModParamSec.nDemoTimeSecs
			? stVidEncModParamMain.nDemoTimeSecs : stVidEncModParamSec.nDemoTimeSecs;		
	}	

	INFO_PRINTF("---------------- Video Rtsp Start -----------------!\n");
	
	while(!g_bRtspStop && nTimeSecs < stVidEncModParam.nDemoTimeSecs)
	{	
		sleep(1);
		nTimeSecs++;
	}

	if (stOsdTitleModParamMain.bEnabled)
	{
		LibXmMaQue_OSD_hideTime(0, MAQUE_STREAM_CHN_MAIN);
		LibXmMaQue_VideoEnc_stopStream(0, MAQUE_STREAM_CHN_MAIN);
	}

	if (stOsdTitleModParamSec.bEnabled)
	{
		LibXmMaQue_OSD_hideTime(0, MAQUE_STREAM_CHN_2END);
		LibXmMaQue_VideoEnc_stopStream(0, MAQUE_STREAM_CHN_2END);
	}

	if (bOsdInited)
	{
		LibXmMaQue_OSD_destroy();
	}
	
	INFO_PRINTF("---------------- Video Rtsp End, eStreamChn: %d, nTimeSecs:%d -----------------!\n", MAQUE_STREAM_CHN_MAIN, nTimeSecs);

	return NULL;
}

static XM_S32 Demo_Rtsp_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bRtspStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Rtsp_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Rtsp_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Rtsp", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("Rtsp bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Rtsp_task, (XM_VOID *)MAQUE_STREAM_CHN_MAIN, "VideoEnc_Main");
		}
	}

	return XM_SUCCESS;
}

