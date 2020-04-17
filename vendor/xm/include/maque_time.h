
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_time.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����ϵͳʱ�����úͻ�ȡ��ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_TIME_H
#define MAQUE_TIME_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


XM_VOID LibXmMaQue_Time_print(MaQueSystemTime_s *pstSysTime, XM_CHAR *pOwer);

XM_S32 LibXmMaQue_Time_syncRtcToSystem(XM_VOID);

/*
������������
	����ϵͳ��ǰʱ�䣬�����RTC�豸��ͬʱ��ͬ����RTC�С�
��������
	  ��������				����
	pstSysTime [in]		ϵͳ��ǰʱ�䡣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Time_setCurrentTime(MaQueSystemTime_s *pstSysTime);

/*
������������
	��ȡϵͳ��ǰʱ�䡣
��������
	  ��������				����
	pstSysTime [out]	�����ȡ��ϵͳ��ǰʱ�䡣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Time_getCurrentTime(MaQueSystemTime_s *pstSysTime);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

