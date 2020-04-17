#include "demo_inc.h"
#include "maque_alarm.h"
#include "demo_alarm.h"

static XM_S32 g_bAlarmStop;

typedef struct
{
	XM_S32 bEnabled;
	XM_U32 nTimes;
	XM_U32 nInterval;
} AlarmModParam_s;


static XM_VOID *Demo_AlarmIn_task(XM_VOID *pArg)
{
	XM_U32 i, count = 0;
	XM_S32 bEnabled = 0;
	XM_S32 alarmInCount;
	AlarmModParam_s stModParam = { 0 };
	StringValueGet_s astValGet[] = 
	{
		{"bEnabled:", &stModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stModParam.bEnabled)},
		{"nTimes:", &stModParam.nTimes, XM_VALUE_TYPE_INT, sizeof(stModParam.nTimes)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AlarmIn", astValGet, sizeof(astValGet) / sizeof(StringValueGet_s));

	alarmInCount = LibXmMaQue_AlarmIn_getCount();

	INFO_PRINTF("alarmInCount:%d\n", alarmInCount);

	if (stModParam.nTimes <= 0)
	{
		stModParam.nTimes = (XM_U32)-1;
	}

	while(!g_bAlarmStop && count <= stModParam.nTimes)
	{
		for (i = 0; i < alarmInCount; i++)
		{
			LibXmMaQue_AlarmIn_getStatus(i, &bEnabled);
		
			DEBUG_PRINTF("AlarmIn #%d, bEnabled: %d\n", i, bEnabled);

			if (bEnabled)
			{
				INFO_PRINTF("AlarmIn#%d, <Enabled>.\n", i);
			}
		}

		count++;

		usleep(40*1000);
	}

	return NULL;
}

static XM_VOID *Demo_AlarmOut_task(XM_VOID *pArg)
{
	XM_U32 i, count = 0;
	XM_S32 bEnabled = 1;
	XM_S32 alarmOutCount;
	AlarmModParam_s stModParam = { 0 };
	StringValueGet_s astValGet[] = 
	{
		{"bEnabled:", &stModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stModParam.bEnabled)},
		{"nTimes:", &stModParam.nTimes, XM_VALUE_TYPE_INT, sizeof(stModParam.nTimes)},
		{"nInterval:", &stModParam.nInterval, XM_VALUE_TYPE_INT, sizeof(stModParam.nInterval)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AlarmOut", astValGet, sizeof(astValGet) / sizeof(StringValueGet_s));

	alarmOutCount = LibXmMaQue_AlarmIn_getCount();

	INFO_PRINTF("alarmOutCount:%d\n", alarmOutCount);

	if (stModParam.nTimes <= 0)
	{
		stModParam.nTimes = (XM_U32)-1;
	}

	while(!g_bAlarmStop && count <= stModParam.nTimes)
	{
		for (i = 0; i < alarmOutCount; i++)
		{
			DEBUG_PRINTF("AlarmOut #%d, bEnabled: %d\n", i, bEnabled);
			
			LibXmMaQue_AlarmOut_enable(i, bEnabled);

			bEnabled = !bEnabled;
		}

		count++;

		sleep(stModParam.nInterval <= 0 ? 1 : stModParam.nInterval);
	}

	return NULL;
}

static XM_S32 Demo_Alarm_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bAlarmStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Alarm_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Alarm_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AlarmIn", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("Alarm In bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_AlarmIn_task, NULL, "AlarmIn");
		}
	}

	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"AlarmOut", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("Alarm Out bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_AlarmOut_task, NULL, "AlarmOut");
		}
	}
	
	return XM_SUCCESS;
}

