
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_gpio.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.05.15
 * ��������: ����GPIO��ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_GPIO_H
#define MAQUE_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_U8 group;
	XM_U8 num;
	XM_U8 value;
} GpioParam_s;


/*
������������
	����GPIO�ߡ�
��������
	  ��������			����
	gpioGroup [in]	GPIO��š�
	gpioNum [in]	GPIO��š�
	pOwnerName [in]	�����߱�ʶ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Gpio_setHigh(XM_U32 gpioGroup, XM_U32 gpioNum, XM_CHAR *pOwnerName);

/*
������������
	����GPIO�͡�
��������
	  ��������			����
	gpioGroup [in]	GPIO��š�
	gpioNum [in]	GPIO��š�
	pOwnerName [in]	�����߱�ʶ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Gpio_setLow(XM_U32 gpioGroup, XM_U32 gpioNum, XM_CHAR *pOwnerName);

/*
������������
	��ȡGPIOֵ��
��������
	  ��������			����
	gpioGroup [in]	GPIO��š�
	gpioNum [in]	GPIO��š�
	pValue [out]	�����ȡ��GPIOֵ��
	pOwnerName [in]	�����߱�ʶ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Gpio_read(XM_U32 gpioGroup, XM_U32 gpioNum, XM_U32 *pValue, XM_CHAR *pOwnerName);


#ifdef __cplusplus
}
#endif

#endif

