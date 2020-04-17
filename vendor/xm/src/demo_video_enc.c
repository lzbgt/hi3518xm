
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_video_encode.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示主辅码流录像。
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
#include "demo_video_enc.h"

#define DEMO_VIDEO_RECORD_DIR	"VideoEncode"
#define DEMO_VIDEO_RECORD_FILE	"VideoRecordStream#%d.h26%d"


typedef struct
{
	FILE *pFile;
	MaQueStreamChannel_e eStreamChn;
	XM_S32 bClosePrint;
} VideoEncCallbackArg_s;


static XM_S32 g_bVideoEncStop;


static EnumStr_s g_astVidStds[MAQUE_VIDEO_STANDARD_NR] =
{
	{MAQUE_VIDEO_STANDARD_PAL, "PAL"},
	{MAQUE_VIDEO_STANDARD_NTSC, "NTSC"},
	{MAQUE_VIDEO_STANDARD_SECAM, "SECAM"},
};

MaQueVideoStandard_e MaQue_Demo_VidStd_str2Enum(XM_CHAR *pName)
{
	return MaQue_Demo_String_toEnum(pName, g_astVidStds, MAQUE_VIDEO_STANDARD_NR);
}


XM_VOID MaQue_Demo_VideoEnc_ModParam_print(XM_CHAR *pOwner, VideoEncodeModParam_s *pstVidEncModParam)
{
	SIMPLE_PRINTF("@%s->Demo_VideoEnc_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstVidEncModParam->bEnabled);
	SIMPLE_PRINTF("\tbWriteFile: %d\n", pstVidEncModParam->bWriteFile);
	SIMPLE_PRINTF("\tbClosePrint: %d\n", pstVidEncModParam->bClosePrint);
	SIMPLE_PRINTF("\tnDemoTimeSecs: %d\n", pstVidEncModParam->nDemoTimeSecs);
	SIMPLE_PRINTF("\taLogoFile: %s\n", pstVidEncModParam->aLogoFile);
	SIMPLE_PRINTF("\taVidComp: %s\n", pstVidEncModParam->aVidComp);
	SIMPLE_PRINTF("\taCapSize: %s\n", pstVidEncModParam->aCapSize);
	SIMPLE_PRINTF("\taBitrateCtrl: %s\n", pstVidEncModParam->aBitrateCtrl);
	SIMPLE_PRINTF("\taImgQuality: %s\n", pstVidEncModParam->aImgQuality);
	SIMPLE_PRINTF("\tnFps: %d\n", pstVidEncModParam->nFps);
	SIMPLE_PRINTF("\tnBitRate: %d\n", pstVidEncModParam->nBitRate);
	SIMPLE_PRINTF("\tnIFrameInterval: %d\n", pstVidEncModParam->nIFrameInterval);
	SIMPLE_PRINTF("\tiQpMin: %d\n", pstVidEncModParam->iQpMin);
	SIMPLE_PRINTF("\tiQpMax: %d\n", pstVidEncModParam->iQpMax);
}

XM_VOID MaQue_Demo_VideoEnc_OsdModParam_print(XM_CHAR *pOwner, OsdTitleModParam_s *pstOsdModParam)
{
	SIMPLE_PRINTF("@%s->Demo_VideoEnc_OsdModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\tbEnabled: %d\n", pstOsdModParam->bEnabled);
	SIMPLE_PRINTF("\tstOsdParam.index: %d\n", pstOsdModParam->stOsdParam.index);
	SIMPLE_PRINTF("\tstOsdParam.left: %d\n", pstOsdModParam->stOsdParam.left);
	SIMPLE_PRINTF("\tstOsdParam.top: %d\n", pstOsdModParam->stOsdParam.top);
	SIMPLE_PRINTF("\tstOsdParam.eFontSize: %d\n", pstOsdModParam->stOsdParam.eFontSize);
	SIMPLE_PRINTF("\tstOsdParam.fgTrans: %d\n", pstOsdModParam->stOsdParam.fgTrans);
	SIMPLE_PRINTF("\tstOsdParam.bgTrans: %d\n", pstOsdModParam->stOsdParam.bgTrans);
	SIMPLE_PRINTF("\tstOsdParam.fgColor: 0x%x\n", pstOsdModParam->stOsdParam.fgColor);
	SIMPLE_PRINTF("\tstOsdParam.bgColor: 0x%x\n", pstOsdModParam->stOsdParam.bgColor);
	SIMPLE_PRINTF("\taFontLibPath: %s\n", pstOsdModParam->aFontLibPath);
	SIMPLE_PRINTF("\taTitle: %s\n", pstOsdModParam->aTitle);
}

XM_S32 MaQue_Demo_VideoEnc_setCfg(MaQueStreamChannel_e eStreamChn, VideoEncodeModParam_s *pstModParam)
{
	XM_S32 res;
	MaQueVideoEncodeCfg_s stEncodeCfg;

	if (LibXmMaQue_VideoEnc_getCfg(0, eStreamChn, &stEncodeCfg))  /* 只是为了演示获取参数用法，实际可能不需要。*/
	{
		ERROR_PRINTF("LibXmMaQue_VideoEncode_getCfg error\n");
		return -1;
	}

	stEncodeCfg.eVidComp = LibXmMaQue_Video_Compress_toEnum(pstModParam->aVidComp);
	stEncodeCfg.eCapSize = LibXmMaQue_Video_CaptureSize_toEnum(pstModParam->aCapSize);	
	stEncodeCfg.eBitrateCtrl = LibXmMaQue_Video_BitrateCtrl_toEnum(pstModParam->aBitrateCtrl);
	stEncodeCfg.eImageQuality = LibXmMaQue_ImageQuality_toEnum(pstModParam->aImgQuality);
	stEncodeCfg.nFps = pstModParam->nFps <= 0 ? 10 : pstModParam->nFps;
	stEncodeCfg.nBitRate = pstModParam->nBitRate <= 0 ? 1536 : pstModParam->nBitRate;
	stEncodeCfg.nIFrameInterval = pstModParam->nIFrameInterval;

	if (pstModParam->iQpMin > 0 && pstModParam->iQpMin <= 51
		&& pstModParam->iQpMax > 0 && pstModParam->iQpMax <= 51)
	{
		stEncodeCfg.iQpMin = pstModParam->iQpMin;
		stEncodeCfg.iQpMax = pstModParam->iQpMax;
		stEncodeCfg.bUseUserQp = 1;
	}

	DEBUG_PRINTF("main: fps=%d\n", stEncodeCfg.nFps);

	res = LibXmMaQue_VideoEnc_setCfg(0, eStreamChn, &stEncodeCfg);

	return res;
}

static XM_S32 Demo_getVideoStream_callback(XM_VOID *pUserArg, MaQueVideoEncFrameInfo_s *pstFrameInfo)
{
	MaQueSystemTime_s *pstSysTime = &pstFrameInfo->stTimeStamp.stSysTime;
	VideoEncCallbackArg_s *pstCallbackArg = (VideoEncCallbackArg_s *)pUserArg;

	if (pstCallbackArg->pFile)
	{
		fwrite(pstFrameInfo->pData, 1, pstFrameInfo->nDataLen, pstCallbackArg->pFile);
	}
	else
	{
		if (!pstCallbackArg->bClosePrint)
		{
			INFO_PRINTF("<%s> Frame,Time: %ds + %dms, pts: %lld <Stream#%d>\n", 
			 	0 == pstFrameInfo->eSubType ? "I" : "P", pstSysTime->second,
				pstFrameInfo->stTimeStamp.mSec, pstFrameInfo->pts, pstCallbackArg->eStreamChn);
		}
	}
	
	if (MAQUE_FRAME_SUBTYPE_I == pstFrameInfo->eSubType)
	{
		DEBUG_PRINTF("<I> Frame, Time: %ds + %dms, pts: %lld <Stream#%d>\n", 
			pstSysTime->second, pstFrameInfo->stTimeStamp.mSec, pstFrameInfo->pts, pstCallbackArg->eStreamChn);
	}
	
	MaQue_Demo_Mem_release(pstFrameInfo->handleMem);

	return XM_SUCCESS;
}

static XM_VOID *Demo_VideoEnc_record_task(XM_VOID *pArg)
{
	XM_S32 res;
	MaQueStreamChannel_e eStreamChn = (MaQueStreamChannel_e)pArg;
	XM_U32 nTimeSecs = 0;
	FILE *pstFile = NULL;
	XM_S32 bOsdInited = 0;
	VideoEncCallbackArg_s stCallbackArg;
	XM_CHAR aRecFile[256];
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
	OsdTitleModParam_s stOsdTimeModParam = { 0 };
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

	if (MAQUE_STREAM_CHN_MAIN == eStreamChn)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"VideoMainChnEncode", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));
	}
	else if (MAQUE_STREAM_CHN_2END == eStreamChn)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"VideoSecondChnEncode", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));
	}

	MaQue_Demo_VideoEnc_ModParam_print("Demo_VideoEnc_record_task()", &stVidEncModParam);

	if (!stVidEncModParam.bEnabled)
	{
		INFO_PRINTF("Video Encode Module Not Enabled, Chn: %d!\n", eStreamChn);	
		return NULL;
	}
	
	res = MaQue_Demo_VideoEnc_setCfg(eStreamChn, &stVidEncModParam);
	if (res < 0)
	{
		ERROR_PRINTF("Demo_VideoEnc_setCfg() Failed, eStreamChn: %d!\n", eStreamChn);
	}

	if (res == 1)
	{
		ERROR_PRINTF("Need to reboot, eStreamChn: %d!\n", eStreamChn);
		return NULL;
	}

	if (stVidEncModParam.bWriteFile)
	{
		XM_CHAR aTmpFile[256];
		XM_CHAR *pEnv = NULL;
		XM_CHAR *pDir = DEMO_VIDEO_RECORD_DIR;

		pEnv = getenv("MAQUE_RECORD_STORE_DIR");
		if (pEnv)
		{
			strcpy(aTmpFile, pEnv);
			INFO_PRINTF("pEnv: %s.\n", pEnv);
			strcat(aTmpFile, "/");
			strcat(aTmpFile, DEMO_VIDEO_RECORD_DIR);
			pDir = aTmpFile;
		}
		
		res = mkdir(pDir, 0755);
		if (res != 0 && EEXIST != errno)
		{
			ERROR_PRINTF("mkdir Failed: %s, Error: %s!\n", pDir, strerror(errno));
			return NULL;
		}
		else
		{
			INFO_PRINTF("mkdir OK: %s!\n", pDir);
		}

		strcpy(aRecFile, pDir);
		strcat(aRecFile, "/");
		sprintf(aTmpFile, DEMO_VIDEO_RECORD_FILE, eStreamChn, !strcasecmp(stVidEncModParam.aVidComp, "H264") ? 4 : 5);
		strcat(aRecFile, aTmpFile);

		INFO_PRINTF("fopen() File: %s!\n", aRecFile);

		pstFile = fopen(aRecFile, "w+");
		if (!pstFile)
		{
			ERROR_PRINTF("fopen() Failed: %s!\n", DEMO_VIDEO_RECORD_FILE);	
			//return NULL;
		}
	}

	stCallbackArg.eStreamChn = eStreamChn;
	stCallbackArg.pFile = pstFile;
	stCallbackArg.bClosePrint = stVidEncModParam.bClosePrint;
		
	LibXmMaQue_VideoEnc_startStream(0, eStreamChn, Demo_getVideoStream_callback, &stCallbackArg);

	if (!access(stVidEncModParam.aLogoFile, F_OK))
	{
		LibXmMaQue_OSD_setLogo(0, eStreamChn, 5, stVidEncModParam.aLogoFile, 2);
	}

	if (MAQUE_STREAM_CHN_MAIN == eStreamChn)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"OsdTimeMainChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
		stOsdTimeModParam = stOsdTitleModParam;

		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"OsdTitleMainChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	}
	else if (MAQUE_STREAM_CHN_2END == eStreamChn)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"OsdTimeSecondChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
		
		stOsdTimeModParam = stOsdTitleModParam;
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"OsdTitleSecondChn", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));
	}

	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTimeModParam", &stOsdTimeModParam);
	MaQue_Demo_VideoEnc_OsdModParam_print("stOsdTitleModParam", &stOsdTitleModParam);

	pEnv = getenv("MAQUE_FONT_FILE");
	if (pEnv)
	{
		INFO_PRINTF("pEnv: %s.\n", pEnv);
	}

	if (stOsdTitleModParam.bEnabled)
	{
		LibXmMaQue_OSD_showTime(0, eStreamChn, &stOsdTimeModParam.stOsdParam);
		
		pFontFile = stOsdTitleModParam.aFontLibPath;
		
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
				LibXmMaQue_OSD_setTitle(0, eStreamChn, &stOsdTitleModParam.stOsdParam, stOsdTitleModParam.aTitle);
				usleep(100 *1000);
			}
	
			bOsdInited = 1;
		}
	}	

	if (stVidEncModParam.nDemoTimeSecs <= 0)
	{
		stVidEncModParam.nDemoTimeSecs = (XM_U32)-1;		
	}

	INFO_PRINTF("---------------- Video Record Start, eStreamChn: %d -----------------!\n", eStreamChn);
	
	while(!g_bVideoEncStop && nTimeSecs < stVidEncModParam.nDemoTimeSecs)
	{	
		sleep(1);
		nTimeSecs++;
	}

	if (stOsdTitleModParam.bEnabled)
	{
		LibXmMaQue_OSD_hideTime(0, eStreamChn);
	}

	LibXmMaQue_VideoEnc_stopStream(0, eStreamChn);

	if (pstFile)
	{
		fflush(pstFile);
		fclose(pstFile);
		pstFile = NULL;
	}

	if (bOsdInited)
	{
		LibXmMaQue_OSD_destroy();
	}
	
	INFO_PRINTF("---------------- Video Record End, eStreamChn: %d, nTimeSecs:%d -----------------!\n", eStreamChn, nTimeSecs);

	return NULL;
}

static XM_S32 Demo_VideoEnc_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bVideoEncStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_VideoEnc_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_VideoEnc_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"VideoMainChnEncode", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("VideoMainChnEncode bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_VideoEnc_record_task, (XM_VOID *)MAQUE_STREAM_CHN_MAIN, "VideoEnc_Main");
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"VideoSecondChnEncode", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("VideoSecondChnEncode bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_VideoEnc_record_task, (XM_VOID *)MAQUE_STREAM_CHN_2END, "VideoEnc_Sec");
		}
	}

	return XM_SUCCESS;
}


