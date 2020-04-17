/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_inc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: Xm MaQue demo��ÿ��.c�ļ����������ͷ�ļ���ֻ��demo�ڲ�ʹ�á�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
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

