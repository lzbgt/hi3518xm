
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_rs485.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.05.15
 * ��������: ����RS485��д��ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_RS485_H
#define MAQUE_RS485_H


#ifdef __cplusplus
extern "C" {
#endif

/// ��������λ��
typedef enum  
{
	MAQUE_UART_DATA_BITS_5 = 5,
	MAQUE_UART_DATA_BITS_6,
	MAQUE_UART_DATA_BITS_7,
	MAQUE_UART_DATA_BITS_8,
	MAQUE_UART_DATA_BITS_NR
} MaQueUartDataBits_e;

/// ����У��λ����
typedef enum  
{
	MAQUE_UART_PARITY_NO,	 ///< ��У��
	MAQUE_UART_PARITY_ODD, 	 ///< ��У��
	MAQUE_UART_PARITY_EVEN,	 ///< żУ��
	MAQUE_UART_MARK,		 ///< У��λʼ��Ϊ1
	MAQUE_UART_SPACE		 ///< У��λʼ��Ϊ0
} MaQueUartParity_e;

/// ����ֹͣλ����
typedef enum 
{
	MAQUE_UART_STOPBITS_ONE,	///< 1 stop bit
	MAQUE_UART_STOPBITS_ONE5,	///< 1.5 stop bit
	MAQUE_UART_STOPBITS_TWO		///< 2 stop bit
} MaQueUartStopBits_e;


/// �������Խṹ
typedef struct
 {
	XM_U32 baudrate;	///< ʵ�ʵĲ�����ֵ��		
	MaQueUartDataBits_e eDataBits;	///< ʵ�ʵ�����λ����ȡUartDataBits_e���͵�ö��ֵ��	
	MaQueUartParity_e eParity;	///< ��żУ��ѡ�ȡUartParity_e���͵�ö��ֵ��	
	MaQueUartStopBits_e eStopBits;	///< ֹͣλ����ȡUartStopBits_e���͵�ö��ֵ��	
	XM_U8 reserved;	///< ����	
} MaQueUartAttr_s;


/*
������������
	����RS485ͨ�Žӿڡ�
��������
	 �ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_RS485_create(XM_VOID);

/*
������������
	����RS485ͨ�����ԡ�
��������
	 ��������			����
	pstAttr [in]	��Ҫ���õ�ͨ�����ԡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/

XM_S32 LibXmMaQue_RS485_setAttr(MaQueUartAttr_s *pstAttr);


/*
������������
	д���ݵ�RS485�����ϡ�
��������
	  ��������			����
	pData [in]		ָ����Ҫд��RS485���ߵ����ݡ�
	dataLen [in]	��Ҫд��RS485���ߵ����ݳ��ȡ�
������ֵ��
	����0			ʵ��д������ݳ��ȡ�
	С�ڵ���0		ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_RS485_write(XM_CHAR *pData, XM_U32 dataLen);

/*
������������
	��RS485�����϶�ȡ���ݡ�
��������
	  ��������			����
	pData [out]		��Ŵ�RS485���߶�ȡ���ݵĻ��档
	dataLenMax [in]	��RS485���߶�ȡ���ݵ���󳤶ȣ�һ��ΪpData��ָ�Ļ����С��
������ֵ��
	����0			ʵ�ʶ�������ݳ��ȡ�
	С�ڵ���0		ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_RS485_read(XM_CHAR *pData, XM_U32 dataLenMax);

/*
������������
	��RS485�����϶�ȡ���ݣ��û����Դ��볬ʱʱ�䣬�ܳ�ʱΪ: timeoutSec�� + timeoutUs΢�롣
��������
	  ��������			����
	pData [out]		��Ŵ�RS485���߶�ȡ���ݵĻ��档
	dataLenMax [in]	��RS485���߶�ȡ���ݵ���󳤶ȣ�һ��ΪpData��ָ�Ļ����С��
	timeoutSec [in]	��ʱʱ���벿�֡�
	timeoutUs [in]	��ʱʱ��΢�벿�֡�
������ֵ��
	����0			ʵ�ʶ�������ݳ��ȡ�
	С�ڵ���0		ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_RS485_readTimeOut(XM_CHAR *pData, XM_U32 dataLenMax, XM_U32 timeoutSec, XM_U32 timeoutUs);

/*
������������
	����RS485ͨ�Žӿڡ�
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_RS485_destroy(XM_VOID);


#ifdef __cplusplus
}
#endif

#endif

