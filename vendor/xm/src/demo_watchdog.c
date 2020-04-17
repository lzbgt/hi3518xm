
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_watchdog.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示看门狗操作相关功能。
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


