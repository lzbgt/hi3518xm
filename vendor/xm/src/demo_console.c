
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_console.c
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: �ṩ����̨��ؽӿڣ����û�����ʾ�����У��������̡�
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


#define CONSOLE_CALLBACK_COUNT_MAX		100

typedef struct
{
	XM_S32 nCountMax;
	XM_S32 nActCount;
	ConsoleCallback_s astConsoleCallback[CONSOLE_CALLBACK_COUNT_MAX];
	pthread_mutex_t mutexConsole;
} ConsoleCallbackMgr_s;


static ConsoleCallbackMgr_s g_stConsoleCallbackMgr =
{
	.nCountMax = CONSOLE_CALLBACK_COUNT_MAX,
	.mutexConsole = PTHREAD_MUTEX_INITIALIZER,
};

static XM_S32 Demo_Console_process_cmd(XM_CHAR *pBuf)
{
	XM_S32 i = 0;
	ConsoleCallback_s *pstConCB;
	
	if (strlen(pBuf) == 0) //���ַ���
	{
		return XM_FAILURE;
	}

	INFO_PRINTF("Cmd: %s\n", pBuf);

	pthread_mutex_lock(&g_stConsoleCallbackMgr.mutexConsole);

	for (i = 0; i < g_stConsoleCallbackMgr.nActCount; i++)
	{
		pstConCB = &g_stConsoleCallbackMgr.astConsoleCallback[i];

		pstConCB->pCallbackFuncPtr(pstConCB->pCallbackArg, pBuf);
	}

	pthread_mutex_unlock(&g_stConsoleCallbackMgr.mutexConsole);

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Console_registerCallback(ConsoleCallback_s *pstCallback)
{
	ConsoleCallback_s *pstConCB;
	
	pthread_mutex_lock(&g_stConsoleCallbackMgr.mutexConsole);
	
	if (g_stConsoleCallbackMgr.nActCount >= g_stConsoleCallbackMgr.nCountMax)
	{
		pthread_mutex_unlock(&g_stConsoleCallbackMgr.mutexConsole);
		INFO_PRINTF("Too Many Callback: %d\n", g_stConsoleCallbackMgr.nActCount);
		return XM_FAILURE;
	}

	pstConCB = &g_stConsoleCallbackMgr.astConsoleCallback[g_stConsoleCallbackMgr.nActCount];

	memcpy(pstConCB, pstCallback, sizeof(ConsoleCallback_s));

	g_stConsoleCallbackMgr.nActCount++;

	pthread_mutex_unlock(&g_stConsoleCallbackMgr.mutexConsole);

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Console_run(XM_VOID)
{
	XM_CHAR	aBuf[64];
	XM_S32 i = 0;
	
	while(1)
	{
		//fflush(stdout);
		memset(aBuf, 0, sizeof(aBuf));
		
		for (i = 0; i < 64; i++)
		{ 
			aBuf[i] = getchar();
			if(aBuf[i] == '\n')
			{
				break;
			}
		}
		
		if(i >= 64)
		{ 
			usleep(100 * 1000);
			continue;
		}
		
		if(i > 0 && aBuf[i - 1] == '\r')
		{
			i--;
		}
		
		aBuf[i] = '\0';
		
		Demo_Console_process_cmd(aBuf);
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Console_OnLine(XM_CHAR *pBuf, S32_S32_CharPP_FuncPtr cb)
{
	XM_CHAR *pValue[16] = {NULL};
	XM_S32 i = 0;
	XM_S32 n = 0;

	pValue[0] = pBuf;
	
	if (strlen(pBuf) == 0) //���ַ���
	{
		return XM_FAILURE;
	}

	if (NULL == cb)
	{
		return XM_FAILURE;
	}

	while (pBuf[i])
	{
		while (pBuf[i] == ' ')
		{
			pBuf[i] = '\0';
			i++;
		}

		if (pBuf[i])
		{
			pValue[n++] = &pBuf[i];
		}

		while (pBuf[i] && pBuf[i] != ' ')
		{
			i++;
		}
	}

	if (n == 1)
	{
		//����ģ����յ��Ĳ����Ǵ�'-'��ʼ��
		pValue[1] = NULL;
	}

	cb(n, pValue);

	return XM_SUCCESS;
}

