
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_common.c
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ʵ����ʾ�����ú�����
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
#include "demo_common.h"


static XM_CHAR g_aRefFile[256] = 
{
	"MaQueReference.cfg"
};

XM_U64 MaQue_Demo_Common_getNowMs(XM_VOID) 
{
    struct timeval tv;
	
    gettimeofday(&tv, NULL);
   
    return (XM_U64)tv.tv_usec/1000 + tv.tv_sec * 1000ULL;
}

XM_VOID MaQue_Demo_Common_setRefFile(XM_CHAR *pRefFile) 
{
	if (!pRefFile)
	{
		return;
	}
	
    strcpy(g_aRefFile, pRefFile);
}

XM_CHAR *MaQue_Demo_Common_getRefFile(XM_VOID) 
{
	INFO_PRINTF("g_aRefFile: %s.\n", g_aRefFile);
    return g_aRefFile;
}

XM_S32 MaQue_Demo_Common_createThread(VoidP_VoidP_FuncPtr pThreadBodyFuncPtr, XM_VOID *pArg, XM_CHAR *pOwner)
{
	XM_S32 res;
	pthread_t thrdId;
	pthread_attr_t stThrdAttr;
	
	pthread_attr_init(&stThrdAttr);
	
	pthread_attr_setdetachstate(&stThrdAttr, PTHREAD_CREATE_DETACHED);
	
	res = pthread_create(&thrdId, &stThrdAttr, pThreadBodyFuncPtr, pArg);

	pthread_attr_destroy(&stThrdAttr);
	
	if(res != 0)
	{
		ERROR_PRINTF("pthread_create() Failed, Owner: %s!\n", !pOwner ? "NULL" : pOwner);
		return XM_FAILURE;
	}
	else
	{
		INFO_PRINTF("pthread_create() ok, thrdId: 0x%lx, Owner: %s!\n", (XM_UL)thrdId, pOwner);
	}

	return XM_SUCCESS;
}

