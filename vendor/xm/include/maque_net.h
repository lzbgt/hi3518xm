
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_net.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: �������������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_NET_H
#define MAQUE_NET_H


#ifdef __cplusplus
extern "C" {
#endif

/*
������������
	���ð��ӵ�IP��ַ���������롣
��������
	  ��������			����	
	pEthName [in]	����ӿ�����
	pHostIp	[in]	�����õ�IP��ַ�����ƣ�192.168.1.100	
	pNetMask [in]	�����õ��������롣���ƣ�255.255.255.0
������ֵ��
	XM_SUCCESS	�ɹ���
	����	ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Net_setHostIP(XM_CHAR *pEthName, XM_CHAR *pHostIp, XM_CHAR *pNetMask, XM_CHAR *pGateway);

/*
������������
	��ȡ���ӵ�IP��ַ���������롣
��������
	  ��������			����
	pEthName [in]	����ӿ�����
	pHostIp	[out]	ָ���ȡ��IP��ַ�����ƣ�192.168.1.100
	hostIpLen [in]	��pHostIp�����Buffer��С��ע�⣺Ӧ������16���ֽڡ�
	pNetMask [out]	ָ���ȡ���������롣���ƣ�255.255.255.0
	netMaskLen [in]	��pNetMask�����Buffer��С��ע�⣺Ӧ������16���ֽڡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Net_getHostIP(XM_CHAR *pEthName, XM_CHAR *pHostIp, XM_S32 hostIpLen, 
			XM_CHAR *pNetMask, XM_S32 netMaskLen, XM_CHAR *pGateway, XM_S32 gatewayLen);

#if 0
/*
������������
	���ð��ӵ����ء�
��������
	  ��������			����
	pEthName [in]	����ӿ�����
	pGateway [in]	�����õ����ء����ƣ�192.168.1.1
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Net_setGateway(XM_CHAR *pEthName, XM_CHAR *pGateway);

/*
������������
	��ȡ���ӵ����ء�
��������
	  ��������			����
	pEthName [in]	����ӿ�����
	pGateway [out]	ָ���ȡ�����ء����ƣ�192.168.1.1
	gatewayLen [in]		��pGateway�����Buffer��С��ע�⣺Ӧ������16���ֽڡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Net_getGateway(XM_CHAR *pEthName, XM_CHAR *pGateway, XM_S32 gatewayLen);
#endif

/*
������������
	��ȡ���õ�����������Mac��
��������
	  ��������			����
	pEthName [in]	����ӿ�����
	pMac [out]		ָ���ȡ��Mac�����ƣ�00:01:23:34:56:78
	macLen [in]		��pMac�����Buffer��С��ע�⣺Ӧ������18���ֽڡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Net_getMac(XM_CHAR *pEthName, XM_CHAR *pMac, XM_S32 macLen);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

