
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_rtc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.05.30
 * ��������: ��ʾRTC������ع��ܡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.05.30
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.05.30
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#include "demo_inc.h"

#include "demo_rs485.h"

static XM_S32 g_bRs485Stop = 0;

typedef struct
{
	XM_S32 bEnabled;
	XM_U32 nTimes;
} Rs485ModParam_s;


static XM_VOID *Demo_Rs485_task(XM_VOID *pArg)
{
	XM_S32 res, count = 0;
	Rs485ModParam_s stModParam = { 0 };
	StringValueGet_s astValGet[] = 
	{
		{"bEnabled:", &stModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stModParam.bEnabled)},
		{"nTimes:", &stModParam.nTimes, XM_VALUE_TYPE_INT, sizeof(stModParam.nTimes)},
	};
	XM_CHAR *pSendStr = "Hi, I'm XmMaQue SDK. Who are you?\r\n";
	XM_CHAR aReadBuf[128];
	MaQueUartAttr_s stAttr = { 0 };

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RS485", astValGet, sizeof(astValGet) / sizeof(StringValueGet_s));

	res = LibXmMaQue_RS485_create();
	if (res != XM_SUCCESS)
	{
		INFO_PRINTF("LibXmMaQue_RS485_create() Failed!\n");	
		return NULL;
	}

	stAttr.baudrate = 115200;
	stAttr.eDataBits = MAQUE_UART_DATA_BITS_8;
	stAttr.eParity = MAQUE_UART_PARITY_NO;
	stAttr.eStopBits = MAQUE_UART_STOPBITS_ONE;
	
	res = LibXmMaQue_RS485_setAttr(&stAttr);
	if (res != XM_SUCCESS)
	{
		INFO_PRINTF("LibXmMaQue_RS485_setAttr() Failed!\n"); 
		return NULL;
	}

	if (stModParam.nTimes <= 0)
	{
		stModParam.nTimes = (XM_U32)-1;
	}

	while(!g_bRs485Stop && count++ <= stModParam.nTimes)
	{
		//INFO_PRINTF("Write: %s!\n", pSendStr); 
		LibXmMaQue_RS485_write(pSendStr, strlen(pSendStr));

		sleep(3);

		memset(aReadBuf, 0, sizeof(aReadBuf));
		res = LibXmMaQue_RS485_read(aReadBuf, sizeof(aReadBuf));
		if (res <= 0) 
		{
			ERROR_PRINTF("Read <Time Out>!\n"); 
			continue;
		}

		INFO_PRINTF("Read: %s!\n", aReadBuf); 

		//sleep(1);
	}

	LibXmMaQue_RS485_destroy();

	return NULL;
}

static XM_S32 Demo_Rs485_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bRs485Stop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_RS485_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Rs485_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"RS485", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("RS485 bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Rs485_task, NULL, "RS485");
		}
	}
	
	return XM_SUCCESS;
}


