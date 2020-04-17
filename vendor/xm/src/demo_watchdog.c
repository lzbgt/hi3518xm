
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_watchdog.c
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ���Ź�������ع��ܡ�
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


static XM_VOID *Demo_Watchdog_task(XM_VOID *pArg)
{
	XM_S32 count = 0;

	LibXmMaQue_Watchdog_enable(1);

	while(1)
	{
		sleep(1);
		
		if (count++ > 5)
		{
			LibXmMaQue_Watchdog_keepAlive();
			count = 0;
		}
	}

	LibXmMaQue_Watchdog_enable(0);

	return NULL;
}

XM_S32 MaQue_Demo_Watchdog_start(XM_VOID)
{

	MaQue_Demo_Common_createThread(Demo_Watchdog_task, NULL, "WatchDog");
	
	return XM_SUCCESS;
}


