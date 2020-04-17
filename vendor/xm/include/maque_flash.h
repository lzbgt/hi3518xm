
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_flash.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����Flash��д�Ȳ�����ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_FLASH_H
#define MAQUE_FLASH_H


#ifdef __cplusplus
extern "C" {
#endif

/*
������������
	��ʼ��Flash������
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ޡ�
*/
XM_S32 LibXmMaQue_Flash_open(XM_VOID);

/*
������������
	��Flashƫ��addr��ַ������ȡ��СΪsize�����ݣ����浽pBuf�С�

��������
	  ��������			����
	pBuf [out]		��������ȡ���ݵ�Bufferָ�롣
	addr [in]		��Ҫ��ȡ��Flashƫ�Ƶ�ַ��
	size [in]		��Ҫ��ȡ�����ݴ�С��
������ֵ��
	����0		ʵ�ʶ�ȡ�����ݴ�С��
	����		ʧ�ܣ���ο������롣
��ע�⡿
	LibXmMaQue_Flash_open ()���óɹ��󣬲ſ���ʹ�ñ�API��
*/
XM_S32 LibXmMaQue_Flash_read(XM_U8 *pBuf, XM_UL addr, XM_U32 size);

/*
������������
	��pBuf��ָ�ռ��СΪsize�����ݣ�д��Flashƫ��addr��ַ����
��������
	  ��������		����
	pBuf [in]	д��Flash�����ݻ��档
	addr [in]	��Flash��ƫ�Ƶ�ַд���ݡ�
	size [in]	��Ҫд������ݴ�С��
������ֵ��
	����0	ʵ��д������ݴ�С��
	����	ʧ�ܣ���ο������롣
��ע�⡿
	LibXmMaQue_Flash_open()���óɹ��󣬲ſ���ʹ�ñ�API
	���ǵ�Ч�ʣ�ʵ��û�а�"��-�޸�-����-д"��������д������ֱ�Ӳ�����д��������
	��������д����Ҫ�������Լ��������ݱ���������Flash�������С������������ܵ�API��
	LibXmDvr_Flash_getEraseSize()��ȡ��
*/
XM_S32 LibXmMaQue_Flash_write(XM_U8 *pBuf, XM_UL addr, XM_U32 size);

/*
������������
	��ȡ��ǰFlash֧�ֵĲ������С��
��������
	�ޡ�
������ֵ��
	����0	Flash֧�ֵĲ������С��һ��Spi Nor Flash�Ĳ������СΪ64KB��
	����	ʧ�ܣ���ο������롣
��ע�⡿
	���ǵ�Ч�ʣ�LibXmDvr_Flash_write()ʵ��û�а�"��-�޸�-����-д"��������д������
	ֱ�Ӳ�����д����������������д����Ҫ�������Լ��������ݱ�����������ʱ����Ҫ
	��ȡFlash֧�ֵĲ������С��
*/
XM_S32 LibXmMaQue_Flash_getEraseSize(XM_VOID);

/*
������������
	��ȡ��ǰFlash��ID��
��������
	�ޡ�
������ֵ��
	-1		ʧ�ܡ�
	����	Flash ID�����磺
  				����8MB��0xEF4017
  				����16MB��0xEF4018
  				����8MB��0xC22017
  				����16MB��0xC22018
*/
XM_S32 LibXmMaQue_Flash_getId(XM_VOID);

/*
������������
	��ȡ��ǰFlash�����ơ�
��������
	�ޡ�
������ֵ��
	NULL	ʧ�ܡ�
	����	Flash ���ơ����磺
 				w25q64/s25fl064k
  				w25q128
��ע�⡿
	�����ں�Flash�������صġ�ÿ��ƽ̨�����в��죬ֻ���ڵ��ԡ�
*/
XM_CHAR *LibXmMaQue_Flash_getName(XM_VOID);

/*
������������
	�ر�Flash������
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Flash_close(XM_VOID);

/*
������������
	��������Ԥ����
��������
	  ��������		����
	addr [in]	������ʼ��ַ��
	size [in]	������С��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ýӿ�ֻ����֧�ֱ���ϵͳ���豸��
*/
XM_S32 LibXmMaQue_Flash_prePart(XM_UL addr, XM_U32 size);

/*
������������
	������������
��������
	  ��������		����
	addr [in]	������ʼ��ַ��
	size [in]	������С��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ýӿ�ֻ����֧�ֱ���ϵͳ���豸��
*/
XM_S32 LibXmMaQue_Flash_postPart(XM_UL addr, XM_U32 size);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_FLASH_H

