
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_wlan.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.04.12
 * ��������: �������߲�����ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.04.12
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.04.12
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_WLAN_H
#define MAQUE_WLAN_H


#ifdef __cplusplus
extern "C" {
#endif

/// ע��::: ���ļ��ṩ�Ľӿ���Ҫ��Ϻ�̨����ҵ��һ��ʵ�֣����̼�ͨ����Ҫ�漰3���ļ�: 
/// 1.  notifyToChangeWIFIMODEFIFO : wifi ״̬�л������ļ�����AP -->STA��STA-->AP��ͨ��ֵ�ο�: MaQueWLanSwitchState_e
/// 2.  WIFIConfigInfo : ap �ȵ���Ϣ�������ο�: MaQueConfigWlan_s
/// 3.  stareadyfifo : station ģʽ����֪ͨ�ļ����ο�: MaQueWpaState_e

// �����ļ�·����LibXmMaQue_System_startUp �е�stStartParam.aWritableDir ָ��
#define FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO "notifyToChangeWIFIMODEFIFO"
#define FILE_WIFI_CONFIG_INFO "WIFIConfigInfo"
#define FILE_STA_READY_FIFO "stareadyfifo"

#define IW_ENCODING_TOKEN_MAX 128

//��ö�� ��wpa_supplicant �����п�������.
typedef enum
{
	WPA_DISCONNECTED,
	WPA_INTERFACE_DISABLED,
	WPA_INACTIVE,
	WPA_SCANNING,
	WPA_AUTHENTICATING,
	WPA_ASSOCIATING,
	WPA_ASSOCIATED,
	WPA_4WAY_HANDSHAKE,
	WPA_GROUP_HANDSHAKE,
	WPA_COMPLETED
} MaQueWpaState_e;

typedef enum
{
	MAQUE_SWITCH_NONE,
	MAQUE_SWITCH_TO_AP,
	MAQUE_SWITCH_TO_STA,
	MAQUE_SWITCH_TO_SEARCH,
	MAQUE_SWITCH_NR
} MaQueWLanSwitchState_e;

typedef enum
{
	MAQUE_WPA_NOT_EXIST = -1,
	MAQUE_WPA_UNLINK = 0,
	MAQUE_WPA_LINK = 1,
	MAQUE_WPA_NR
} MaQueWpaLinkState_e;

typedef enum
{
	MAQUE_ENCRY_NONE,
	MAQUE_ENCRY_WEP_OPEN,
	MAQUE_ENCRY_AES,
	MAQUE_ENCRY_TKIP,
	MAQUE_ENCRY_NR
} MaQueEncrypType_e;

typedef enum
{
	MAQUE_KEY_HEX,
	MAQUE_KEY_ASCII,
	MAQUE_KEY_NR
} MaQueKeyType_e;

typedef struct 
{
	XM_CHAR aSsid[32];
	XM_S32 auth;
	MaQueEncrypType_e eEncryp;
	MaQueKeyType_e keyType;  	//0:Hex 1:ASCII
	XM_CHAR aPasswd[IW_ENCODING_TOKEN_MAX];
	XM_CHAR aMac[8];
	XM_S32 chn;
	XM_S32 rssi;
	XM_S32 aReserved[3];
} MaQueConfigWlan_s;

typedef struct
{
    XM_S32 count;  //��Ѱ����wifi��������
    MaQueConfigWlan_s aNet[128];//ÿ��wifi����������Ϣ
} MaQueWlanSearchInfo_s;

/*
������������
	����wifi �ȵ㡣ע��: �ýӿ���Ҫ��֪ͨ��̨�������̷����������ܲ��ҽ��������д��/var/tmp/iwlistinfo �ļ���
	֮���ɱ��ӿڽ��������ļ�FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO д��ö��MAQUE_SWITCH_TO_SEARCH ��Ϊ֪ͨ��������
	�����������ܣ�����������Ҫ����Ӧ����
��������
	  ��������			����
	pstWlanSearchInfo [out]	wifi �ȵ��������ο�MaQueWlanSearchInfo_s��
	len [out]		��ʱ��Ч��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Wlan_searchWIFI(MaQueWlanSearchInfo_s *pstWlanSearchInfo, XM_S32 len);

/*
������������
	��ȡAP ����״̬��ע��: ������̨������Ҫ��wpa ������״̬(MaQueWpaState_e) д���ļ�: /var/tmp/wpa_state ��
��������
	  ��������			����
	pEthName [in]	����ӿ�����Ĭ��eth2 ��
������ֵ��
	AP ����״̬���ο�MaQueWpaLinkState_e ��
*/
MaQueWpaLinkState_e LibXmMaQue_Wlan_getWpaStatus(XM_CHAR *pEthName);

/*
������������
	wifi �л���AP ģʽ��ע��: �ýӿ���Ҫ�Ȱ�AP �ȵ�������Ϣд���ļ�FILE_WIFI_CONFIG_INFO����ͨ����
	��FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO д��ö��MAQUE_SWITCH_TO_AP ֪ͨ������������Ӧ����
��������
	  ��������			����
	pstWlan [in]	wifi �ȵ��������ο�MaQueConfigWlan_s��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Wlan_switch2AP(MaQueConfigWlan_s *pstWlan);

/*
������������
	wifi �л���STA ģʽ��ע��: �ýӿ���Ҫ�Ȱ�AP �ȵ�������Ϣд���ļ�FILE_WIFI_CONFIG_INFO����ͨ����
	��FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO д��ö��MAQUE_SWITCH_TO_STA ֪ͨ������������Ӧ����
��������
	  ��������			����
	pstWlan [in]	wifi �ȵ��������ο�MaQueConfigWlan_s��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Wlan_switch2STA(MaQueConfigWlan_s *pstWlan);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

