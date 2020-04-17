
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_watchdog.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: �������Ź�������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_WATCH_DOG_H
#define MAQUE_WATCH_DOG_H


#ifdef __cplusplus
extern "C" {
#endif

/*
������������
	ʹ�ܻ�ֹͣ���Ź���
��������
	  ��������			����
	bEnabled [in]	ʹ�ܻ�ֹͣ���Ź���1��ʹ�ܣ�0��ֹͣ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	���Ź�Ĭ����ֹͣ��Ҫʹ�ÿ��Ź��������ñ�APIʹ�ܡ�
*/
XM_S32 LibXmMaQue_Watchdog_enable(XM_S32 bEnabled);

/*
������������
	���Ź������ι����
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	���Ź�ʹ�ܺ�Ĭ�ϳ�ʱʱ����1���ӣ�Ҫ��1�����ڲ����ñ�APIι�����ͻᴥ������������
*/
XM_S32 LibXmMaQue_Watchdog_keepAlive(XM_VOID);

/*
������������
	���ÿ��Ź�����������ʱʱ�䡣һ���ɹ����ñ�API�������ÿ��Ź���ʱʱ��ΪtimeSecs�룬
	ͬʱ�ܾ�ι�����Ҳ����˵timeSecs��󣬿��Ź���ʱ������ϵͳ��������API��Ҫ����
	������ʱ����ϵͳʱ�䡣
��������
	  ��������			����
	timeSecs [in]	���Ź�����������ʱʱ�䣬��λ���롣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Watchdog_setTimeOutSecs(XM_S32 timeSecs);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_WATCH_DOG_H


