
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_motion_detect.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: �����ƶ�������Ƶ�ڵ���ؽṹ�壬������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_MD_H
#define MAQUE_MD_H


#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	MAQUE_MD_ALARM_TYPE = 7,
} MaQueMdAlarmType_e;

typedef struct 
{
	MaQueMdAlarmType_e eAlarmType;  /* �ƶ���ⱨ�����͡� */
	XM_S32 alarmState;   /* ����״ֵ̬��1���б�����0���ޱ�����*/
	XM_CHAR *pAlarmFileName;   /* ������*/
} MaQueMdAlarm_s;

typedef XM_S32 (*MaQueMdAlarmCallbackFuncPtr)(XM_VOID *pUserArg, MaQueMdAlarm_s *pstMdAlarm);

typedef enum
{
	MAQUE_MD_ALARM_LEVEL_1 = 1,
	MAQUE_MD_ALARM_LEVEL_2,
	MAQUE_MD_ALARM_LEVEL_3,
	MAQUE_MD_ALARM_LEVEL_4,
	MAQUE_MD_ALARM_LEVEL_5,
	MAQUE_MD_ALARM_LEVEL_6,
	MAQUE_MD_ALARM_LEVEL_MAX,
} MaQueMdAlarmLevel_e;

typedef enum
{
	MAQUE_BD_ALARM_LEVEL_1 = 1,
	MAQUE_BD_ALARM_LEVEL_2,
	MAQUE_BD_ALARM_LEVEL_3,
	MAQUE_BD_ALARM_LEVEL_4,
	MAQUE_BD_ALARM_LEVEL_5,
	MAQUE_BD_ALARM_LEVEL_6,
	MAQUE_BD_ALARM_LEVEL_MAX,
} MaQueBdAlarmLevel_e;


typedef struct
{
	XM_S32 bEnabled;   /* �ƶ����ʹ�ܿ��ء�1��ʹ�ܣ�0���رա�*/
	XM_U32 x;   /* �ƶ����������ʼ�����ꡣ���CIF��352 * 288������ϵ��*/
	XM_U32 y;   /* �ƶ����������ʼ�����ꡣ���CIF��352 * 288������ϵ��*/
	XM_U32 w;   /* �ƶ������������CIF��352 * 288������ϵ��*/
	XM_U32 h;   /* �ƶ��������ߡ����CIF��352 * 288������ϵ��*/
	MaQueMdAlarmLevel_e eMdAlarmlevel;			/* �ƶ���������ȵȼ���ֵԽ��Խ������*/
} MaQueMdParam_s;

typedef struct
{
	XM_S32 bEnabled;  /* ��Ƶ�ڵ�ʹ�ܿ��ء�1��ʹ�ܣ�0���رա� */
	MaQueBdAlarmLevel_e eBdAlarmLevel;  /* ��Ƶ�ڵ������ȵȼ���ֵԽ��Խ������ */
}MaQueBdParam_s;


typedef struct
{
	XM_S32 state;   /* ��Ƶ�ڵ�״̬��1�����ڵ���0�����ڵ��� */
} MaQueBdResult_s;

typedef XM_S32(*MaQueBdGetResultFuncPtr)(XM_VOID *pUserArg, MaQueBdResult_s *pstBdRes);

/*
������������
	�����ƶ��������
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MD_create(XM_S32 channel);

/*
������������
	�����ƶ���������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstMdParam [in]		�����õ��ƶ���������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MD_setParam(XM_S32 channel, MaQueMdParam_s *pstMdParam);
/*
������������
	��ȡ�ƶ���������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstMdParam [out]	��������ȡ���ƶ���������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MD_getParam(XM_S32 channel, MaQueMdParam_s *pstMdParam);

/*
������������
	ע���ȡ�ƶ���ⱨ���ص�������
��������
	  ��������					����
	channel	 [in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pCallbackFuncPtr [in]	��ȡ�ƶ���ⱨ���Ļص�����ָ�롣
	pCallbackArg [in]		ע���ߴ����˽�в�����������pCallbackFuncPtr�ĵ�һ��������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MDAlarm_register(XM_S32 channel, MaQueMdAlarmCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);
/*
������������
	�����ƶ��������
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MD_destroy(XM_S32 channel);

/*
������������
	������Ƶ�ڵ��������
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_BD_create(XM_S32 channel);

/*
������������
	������Ƶ�ڵ�������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstBdParam [in]		�����õ���Ƶ�ڵ�������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_BD_setParam(XM_S32 channel, MaQueBdParam_s *pstBdParam);

/*
������������
	��ȡ��Ƶ�ڵ�������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstBdParam	[out]	�����õ���Ƶ�ڵ�������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_BD_getParam(XM_S32 channel, MaQueBdParam_s *pstBdParam);

/*
������������
	ע���ȡ��Ƶ�ڵ�����Ļص�������
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pCallbackFuncPtr [in]	��ȡ��Ƶ�ڵ�����Ļص�����ָ�롣
	pCallbackArg [in]		ע���ߴ����˽�в�����������pCallbackFuncPtr�ĵ�һ��������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_BD_register(XM_S32 channel, MaQueBdGetResultFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
������������
	������Ƶ�ڵ��������
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_BD_destroy(XM_S32 channel);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MD_H


