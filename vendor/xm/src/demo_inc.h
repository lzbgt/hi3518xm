/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_inc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: Xm MaQue demo中每个.c文件必需包含的头文件。只供demo内部使用。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_INC_H
#define MAQUE_DEMO_INC_H


#include "libxmmaque_api.h"
#include "demo_common.h"
#include "demo_string.h"
#include "demo_file.h"
#include "demo_console.h"


extern XM_S32 g_bMaQueDemoDebugOn;

#define DEBUG_PRINTF(string,args...) \
	do	\
	{	\
		if (g_bMaQueDemoDebugOn)	\
		{	\
			DEBUG_PRINTF_INTERNAL(string, ##args);	\
		}	\
	} while(0)

#endif

