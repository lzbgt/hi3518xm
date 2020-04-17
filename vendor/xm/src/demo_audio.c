
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_audio.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ¼������Ƶ���Ź��ܡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#include "demo_inc.h"
#include "demo_audio.h"


#define DEMO_AUDIO_RECORD_DIR	"AudioRecord"
#define DEMO_AUDIO_PLAY_DIR		"AudioPlay"
#define DEMO_AUDIO_PLAY_PCM_DIR		"AudioPlayPcm"

#define DEMO_AUDIO_RECORD_FILE	"AudioRecord#%d.g711%s"


typedef struct
{
	FILE *pFile;
	XM_S32 bClosePrint;
} AudioRecordCallbackArg_s;

typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bWriteFile;
	XM_CHAR aAudioFile[256];	
	XM_CHAR aAudioEncType[32];
} AudioPlayModParam_s;


static XM_S32 g_bAudioStop;


static EnumStr_s g_astAudioEncTypes[2] =
{
	{MAQUE_AUDIO_ENCODE_G711_ALAW, "g711a"},
	{MAQUE_AUDIO_ENCODE_G711_ULAW, "g711u"},
};

static  MaQueAudioEncodeType_e Demo_Audio_EncType_str2Enum(XM_CHAR *pName)
{
	return MaQue_Demo_String_toEnum(pName, g_astAudioEncTypes, 2);
}

static XM_S32 Demo_Audio_record_callback(XM_VOID *pUserArg, MaQueAudioFrameInfo_s *pstFrameInfo)
{
	MaQueTimeStamp_s *pstTimeStamp = &pstFrameInfo->stTimeStamp;
	MaQueSystemTime_s *pstSysTime = &pstTimeStamp->stSysTime;
	AudioRecordCallbackArg_s *pstCallbackArg = (AudioRecordCallbackArg_s *)pUserArg;
	
	if (!pstCallbackArg->pFile)  /* �������Ҫд�ļ���������ӡÿ֡ʱ����Ϣ����ȡ����֡��ϵͳʱ��: ��ͺ��� */
	{  
		if (!pstCallbackArg->bClosePrint)
		{
			INFO_PRINTF("Frame,Time: %ds + %dms\n", pstSysTime->second, pstFrameInfo->stTimeStamp.mSec);
		}

		return XM_SUCCESS;
	}
	
	fwrite(pstFrameInfo->pData, 1, pstFrameInfo->nDataLen, pstCallbackArg->pFile);
	
	return XM_SUCCESS;
}

static XM_VOID *Demo_Audio_play_task(XM_VOID *pArg)
{
	XM_S32 res;
	FILE* pFile = NULL;
	XM_U8 aBuf[64*1024];
	XM_S32 readLen = 0;
	XM_S32 maxLen = 160 * 100;
	XM_CHAR aFileName[256];
	MaQueAoChnState_s stAoStatus = { 0 };
	MaQueAudioDecodeFormat_s stAudDecFormat;
	AudioPlayModParam_s stAudModParam = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stAudModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bEnabled)},
		{"bWriteFile:", &stAudModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bWriteFile)},
		{"AudioFile:", &stAudModParam.aAudioFile, XM_VALUE_TYPE_STRING, sizeof(stAudModParam.aAudioFile)},	
		{"AudioEncType:", &stAudModParam.aAudioEncType, XM_VALUE_TYPE_STRING, sizeof(stAudModParam.aAudioEncType)},
	};
	XM_CHAR *pEnv = NULL;

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioPlay", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));
	

	maxLen = (maxLen / 160) * 160;   /* ÿ�ΰ�֡���ȶ��봫�����ݣ��ɼ����м临�ƣ����Ч�ʡ�g711a��g711uÿ֡
									  * 80��������λ��2�ֽڣ���160�ֽڡ�
									  */

	if (maxLen < 160)
	{
		ERROR_PRINTF("Invalid maxLen: %d < 160.\n", 160);
		return NULL;
	}
	
	INFO_PRINTF(" -----------  Audio Play Start ----------------	\n");
	
	readLen = 0;

	pEnv = getenv("MAQUE_AUDIO_PLAY_FILE");
	if (pEnv)
	{
		strcpy(aFileName, pEnv);
		INFO_PRINTF("pEnv: %s.\n", pEnv);
	}
	else
	{
		strcpy(aFileName, DEMO_AUDIO_PLAY_DIR);
		strcat(aFileName, "/");
		strcat(aFileName, stAudModParam.aAudioFile);
	}

	pFile = fopen(aFileName, "r");
	if (!pFile)
	{
		ERROR_PRINTF("fopen() Failed, %s\n", aFileName);
		return NULL;
	}

	stAudDecFormat.eEncType = Demo_Audio_EncType_str2Enum(stAudModParam.aAudioEncType);
	//stAudDecFormat.eEncType = MAQUE_AUDIO_ENCODE_G711_ALAW; 
	stAudDecFormat.eSampleRate = MAQUE_AUDIO_SAMPLE_RATE_8000; 
	stAudDecFormat.eSampleBits = MAQUE_AUDIO_SAMPLE_BITS_16; 
	stAudDecFormat.bitrateKb = 0;  /* G711A��G711U����Ҫ���ñ����ʡ�*/

	res = LibXmMaQue_AudioDec_create(0, &stAudDecFormat);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_AudioDec_create() Failed!\n");
		fclose(pFile);
		return NULL;
	}

	LibXmMaQue_Audio_setSpeakerVolume(1, 3);
	
	while (!g_bAudioStop && (readLen = fread(aBuf, 1, maxLen, pFile)) > 0)
	{	
		do
		{
			res = LibXmMaQue_AudioDec_decode(0, aBuf, readLen);
			usleep(40000);
			
		} while(XM_AGAIN == res);  /* �����ݸ�������������buffer����������XM_AGAIN����ϣ�����������ԡ�*/

		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("LibXmMaQue_AudioDec_decode() Failed!\n");
		}
		
		usleep(1000);
	}

	fclose(pFile);

	while (!g_bAudioStop) /* Ҫ���ǲ����������˳�������ֻ�е���Ƶ���ͨ���ͽ������ݶ��ж���ʱ�����˳���*/
	{	
		res = LibXmMaQue_AudioOut_getChnStat(0, &stAoStatus);  /* �ж���Ƶ���ͨ���Ƿ�������û�в����ꡣ*/
		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("LibXmMaQue_AudioOut_getChnStat() Failed!\n");
			//break;
		}

		DEBUG_PRINTF("Audio Out Chn: nChnTotalNum: %d, nChnFreeNum: %d, nChnBusyNum: %d!\n", 
			stAoStatus.nChnTotalNum, stAoStatus.nChnFreeNum, stAoStatus.nChnBusyNum);

		if ((res = LibXmMaQue_AudioDec_getBufferedLen(0)) <= 0 
			&& stAoStatus.nChnBusyNum <= 0)  /* �жϽ������ݶ������Ƿ�������û�н��롣*/
		{
			INFO_PRINTF("LibXmMaQue_AudioDec_getBufferedLen() < 0, res: %d!\n", res);
			break;
		}

		usleep(10 * 1000);
	}

	LibXmMaQue_AudioDec_destroy(0);

	INFO_PRINTF(" -----------  Audio Play End ----------------	\n");
	
	return NULL;
}

static XM_VOID *Demo_Audio_playPcm_task(XM_VOID *pArg)
{
	XM_S32 res;
	FILE* pFile = NULL;
	XM_U8 aBuf[512*1024];
	XM_S32 readLen = 0;
	XM_S32 maxLen = 50*1024;
	XM_CHAR aFileName[256];
	MaQueAudioDecodeFormat_s stAudDecFormat;
	MaQueAoChnState_s stAoStatus = { 0 };
	AudioPlayModParam_s stAudModParam = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stAudModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bEnabled)},
		{"bWriteFile:", &stAudModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bWriteFile)},
		{"AudioFile:", &stAudModParam.aAudioFile, XM_VALUE_TYPE_STRING, sizeof(stAudModParam.aAudioFile)},	
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioPlayPcm", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	maxLen = (maxLen / 320) * 320;

	if (maxLen < 320)
	{
		ERROR_PRINTF("Invalid maxLen: %d < 320.\n", maxLen);
		return NULL;
	}
	
	INFO_PRINTF(" -----------  Audio Play Pcm Start----------------	\n");
	
	readLen = 0;

	strcpy(aFileName, DEMO_AUDIO_PLAY_PCM_DIR);
	strcat(aFileName, "/");
	strcat(aFileName, stAudModParam.aAudioFile);
		
	
	pFile = fopen(aFileName, "r");
	if (!pFile)
	{
		ERROR_PRINTF("fopen() Failed, %s\n", aFileName);
		return NULL;
	}

	stAudDecFormat.eEncType = MAQUE_AUDIO_ENCODE_G711_ALAW;
	stAudDecFormat.eSampleRate = MAQUE_AUDIO_SAMPLE_RATE_8000; 
	stAudDecFormat.eSampleBits = MAQUE_AUDIO_SAMPLE_BITS_16; 
	stAudDecFormat.bitrateKb = 0;  /* G711A��G711U����Ҫ���ñ����ʡ�*/

	LibXmMaQue_AudioDec_create(0, &stAudDecFormat);
	
	while (!g_bAudioStop && (readLen = fread(aBuf, 1, maxLen, pFile)) > 0)
	{	
		res = LibXmMaQue_AudioOut_sendPcmData(0, aBuf, readLen);
		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("LibXmMaQue_AudioOut_sendPcmData() Failed!\n");
			break;
		}
		
		usleep(1000);
	}

	fclose(pFile);

	while (!g_bAudioStop)
	{	
		res = LibXmMaQue_AudioOut_getChnStat(0, &stAoStatus);
		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("LibXmMaQue_AudioOut_getChnStat() Failed!\n");
			break;
		}

		DEBUG_PRINTF("Audio Out Chn: nChnTotalNum: %d, nChnFreeNum: %d, nChnBusyNum: %d!\n", 
			stAoStatus.nChnTotalNum, stAoStatus.nChnFreeNum, stAoStatus.nChnBusyNum);

		if (stAoStatus.nChnBusyNum <= 0)
		{
			INFO_PRINTF("if (stAoStatus.nChnBusyNum <= 0) <Ok>!\n");
			break;
		}

		usleep(10 * 1000);
	}

	LibXmMaQue_AudioDec_destroy(0);

	INFO_PRINTF(" -----------  Audio Play Pcm End----------------	\n");
	
	return NULL;
}


static XM_VOID *Demo_Audio_record_task(XM_VOID *pArg)
{
	XM_S32 res = XM_FAILURE;
	FILE *pFile = NULL;
	XM_CHAR aFileName[256];
	MaQueAudioEncodeFormat_s stAudEncFormat = { 0 };
	AudioRecordCallbackArg_s stCallbackArg;
	XM_S32 nTimeSecs = 0;
	AudioRecordModParam_s stAudModParam = { 0 };
	StringValueGet_s astVidEncValGet[] = 
	{
		{"bEnabled:", &stAudModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bEnabled)},
		{"bWriteFile:", &stAudModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bWriteFile)},
		{"bClosePrint:", &stAudModParam.bClosePrint, XM_VALUE_TYPE_INT, sizeof(stAudModParam.bClosePrint)},
		{"nDemoTimeSecs:", &stAudModParam.nDemoTimeSecs, XM_VALUE_TYPE_INT, sizeof(stAudModParam.nDemoTimeSecs)},	
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioRecord", astVidEncValGet, sizeof(astVidEncValGet) / sizeof(StringValueGet_s));

	//LibXmMaQue_Audio_setMicVolume(1, (30 << 8) | 10);

	//LibXmMaQue_Audio_setSpeakerVolume(1, 15);

	INFO_PRINTF(" ------------ Audio Record Start ---------------\n");

	INFO_PRINTF("bWriteFile: %d, nDemoTimeSecs: %d.\n", stAudModParam.bWriteFile, stAudModParam.nDemoTimeSecs);

	if (stAudModParam.bWriteFile)
	{
		XM_CHAR aTmpFile[256];
		XM_CHAR *pEnv = NULL;
		XM_CHAR *pDir = DEMO_AUDIO_RECORD_DIR;

		pEnv = getenv("MAQUE_RECORD_STORE_DIR");
		if (pEnv)
		{
			strcpy(aTmpFile, pEnv);
			INFO_PRINTF("pEnv: %s.\n", pEnv);
			strcat(aTmpFile, "/");
			strcat(aTmpFile, DEMO_AUDIO_RECORD_DIR);
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

		strcpy(aFileName, pDir);
		strcat(aFileName, "/");
		sprintf(aTmpFile, DEMO_AUDIO_RECORD_FILE, 0, "a");
		strcat(aFileName, aTmpFile);

		pFile = fopen(aFileName, "w+");

		if (!pFile)
		{
			ERROR_PRINTF("fopen() Failed: %s\n", aFileName);
			//return NULL;
		}
	}

	stAudEncFormat.eEncType = MAQUE_AUDIO_ENCODE_G711_ALAW; 
	stAudEncFormat.eSampleRate = MAQUE_AUDIO_SAMPLE_RATE_8000; 
	stAudEncFormat.eSampleBits = MAQUE_AUDIO_SAMPLE_BITS_16; 
	stAudEncFormat.bitrateKb = 0;  /* G711A��G711U����Ҫ���ñ����ʡ�*/
	
	stCallbackArg.pFile = pFile;
	stCallbackArg.bClosePrint = stAudModParam.bClosePrint;
	
	res = LibXmMaQue_AudioEnc_create(0, &stAudEncFormat, Demo_Audio_record_callback, &stCallbackArg);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_AudioEnc_create() Failed!\n");
		fclose(pFile);
		return NULL;
	}

	LibXmMaQue_Audio_setMicVolume(1, (30 << 8) | 46);
	
	if (stAudModParam.nDemoTimeSecs <= 0)
	{
		stAudModParam.nDemoTimeSecs = (XM_U32)-1;
	}

	while(!g_bAudioStop && nTimeSecs < stAudModParam.nDemoTimeSecs)
	{
		sleep(1);
		nTimeSecs++;
	}

	INFO_PRINTF(" ------------ Audio Record End, nTimeSecs: %d ---------------\n", nTimeSecs);

	LibXmMaQue_AudioEnc_destroy(0);

	if (pFile)
	{
		fflush(pFile);
		fclose(pFile);
	}

	return NULL;
}

static XM_S32 Demo_Audio_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bAudioStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Audio_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Audio_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioRecord", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Audio_record_task, (XM_VOID *)1, "Audio_Record");
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioPlay", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Audio_play_task, (XM_VOID *)0, "Audio_Play");
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AudioPlayPcm", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Audio_playPcm_task, (XM_VOID *)0, "Audio_PlayPcm");
		}
	}

	return XM_SUCCESS;
}



