
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_camera.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ������������ͷ��ز����ṹ�壬������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_CAMERA_H
#define MAQUE_CAMERA_H


#ifdef __cplusplus
extern "C" {
#endif


typedef XM_S32(*MaQueIrCutCallbackFuncPtr) (XM_VOID *pUserArg, XM_S32 bNightMode);
typedef XM_S32(*MaQueDayNightCallbackFuncPtr) (XM_VOID *pUserArg, XM_S32 bNightMode);

typedef enum
{
	MAQUE_WB_MODE_AUTO,  	/* �Զ���ƽ��ģʽ */
	MAQUE_WB_MODE_INDOOR,  	/* ���ڡ��ֶ���ƽ�⣬�ݲ�֧�֡� */
	MAQUE_WB_MODE_OUTDOOR   /* ���⡣�ֶ���ƽ�⣬�ݲ�֧�֡�  */
} MaQueWhiteBalanceMode_e;

typedef enum
{
	MAQUE_DNC_MODE_AUTO,			// �Զ��л�
	MAQUE_DNC_MODE_MULTICOLOR,		// ǿ��Ϊ��ɫ
	MAQUE_DNC_MODE_BLACKWHITE,		// ǿ��Ϊ�ڰ�ģʽ
	MAQUE_DNC_MODE_BUTT
} MaQueDncMode_e;  				// �����ҹ��ɫ�л�ģʽ��

typedef enum 
{
	MAQUE_DNC_THRESHOLD_LEVEL_1 = 1,	/* �����ҹ��ɫ�л���ֵ�ȼ�1���ȼ�ԽС�л�Խ������*/
	MAQUE_DNC_THRESHOLD_LEVEL_2,    	/* �����ҹ��ɫ�л���ֵ�ȼ�2��*/
	MAQUE_DNC_THRESHOLD_LEVEL_3,    	/* �����ҹ��ɫ�л���ֵ�ȼ�3��*/
	MAQUE_DNC_THRESHOLD_LEVEL_4,    	/* �����ҹ��ɫ�л���ֵ�ȼ�4��*/
	MAQUE_DNC_THRESHOLD_LEVEL_5,    	/* �����ҹ��ɫ�л���ֵ�ȼ�5��*/
} MaQueDncThresholdLevel_e;

typedef enum
{
    MAQUE_OP_TYPE_AUTO,     /* �Զ���*/
    MAQUE_OP_TYPE_MANUAL,   /* �ֶ���*/
    MAQUE_OP_TYPE_BUTT
} MaQueOperationType_e;

typedef enum 
{
	MAQUE_IRCUT_SWITCH_SYN_INFRARED,	/* ������ͬ����*/
	MAQUE_IRCUT_SWITCH_AUTO,			/* ���ݻ����Զ���*/
} MaQueIrCutMode_e;

typedef enum 
{
	MAQUE_MANUAL_EXP_LEVEL_1 = 1,		/* 1/50 �� */
	MAQUE_MANUAL_EXP_LEVEL_2,    		/* 1/120 �� */
	MAQUE_MANUAL_EXP_LEVEL_3,    		/* 1/250 �� */
	MAQUE_MANUAL_EXP_LEVEL_4,    		/* 1/500 �� */
	MAQUE_MANUAL_EXP_LEVEL_5,    		/* 1/1000 �� */
	MAQUE_MANUAL_EXP_LEVEL_6,    		/* 1/2000 �� */
	MAQUE_MANUAL_EXP_LEVEL_7,    		/* 1/4000 �� */
	MAQUE_MANUAL_EXP_LEVEL_8,    		/* 1/10000 �� */
} MaQueManualExpLevel_e;

typedef enum 
{
	MAQUE_ES_SHUTTER_LEVEL_NONE,	/* ���������Ų�������*/	
	MAQUE_ES_SHUTTER_LEVEL_1,		/* ���������ŵȼ�1��֡������ΪSensor���֡�ʵ�4/5�� */
	MAQUE_ES_SHUTTER_LEVEL_2,    	/* ���������ŵȼ�2��֡������ΪSensor���֡�ʵ�1/2������ع�ʱ��͵ȼ�1��ͬ�� */
	MAQUE_ES_SHUTTER_LEVEL_3,    	/* ���������ŵȼ�3��֡������ΪSensor���֡�ʵ�1/2������ع�ʱ��ȵȼ�2�� */
	MAQUE_ES_SHUTTER_LEVEL_4,    	/* ���������ŵȼ�4��֡������ΪSensor���֡�ʵ�1/3������ع�ʱ�����һ�ȼ��� */
	MAQUE_ES_SHUTTER_LEVEL_5,    	/* ���������ŵȼ�5��֡������ΪSensor���֡�ʵ�1/4������ع�ʱ�����һ�ȼ��� */
	MAQUE_ES_SHUTTER_LEVEL_BUTT, 
} MaQueEsShutterLevel_e;

typedef enum 
{
	MAQUE_NOISE_FILTER_LEVEL_NONE,	/* �����˲���������*/	
	MAQUE_NOISE_FILTER_LEVEL_1,		/* �����˲��ȼ�1���ȼ�Խ�󣬽���Խ���ԡ�*/
	MAQUE_NOISE_FILTER_LEVEL_2,    	/* �����˲��ȼ�2���ȼ�Խ�󣬽���Խ���ԡ�*/
	MAQUE_NOISE_FILTER_LEVEL_3,    	/* �����˲��ȼ�3���ȼ�Խ�󣬽���Խ���ԡ�*/
	MAQUE_NOISE_FILTER_LEVEL_4,    	/* �����˲��ȼ�4���ȼ�Խ�󣬽���Խ���ԡ�*/
	MAQUE_NOISE_FILTER_LEVEL_5,    	/* �����˲��ȼ�5���ȼ�Խ�󣬽���Խ���ԡ�*/
	MAQUE_NOISE_FILTER_LEVEL_BUTT, 
} MaQueNoiseFilterLevel_e;

typedef enum 
{
	MAQUE_INFRARED_DAY,
	MAQUE_INFRARED_NIGHT,
	MAQUE_INFRARED_BUTT,
} MaQueInfraredState_e;

typedef enum
{
	MAQUE_LED_TYPE_IR,  /* ����ơ�*/
	MAQUE_LED_TYPE_WL,  /* �׹�ơ�*/
	MAQUE_LED_TYPE_BUTT,
} MaQueLedType_e;


typedef enum
{
	MAQUE_LED_MODE_AUTO,
	MAQUE_LED_MODE_MANUAL,
	MAQUE_LED_MODE_INTELL,
	MAQUE_LED_MODE_BUTT,
} MaQueLedMode_e;

typedef struct
{
	MaQueDncMode_e eDncMode;        	 			// ��ҹ��ɫģʽ��ȡֵ�в�ɫ���Զ��л��ͺڰ�
	MaQueDncThresholdLevel_e eDncThresholdLevel;	// ��ҹת����ֵ�ȼ���ÿ���ȼ���Ӧ��ͬ���ع�����ʵ�ʲο����ع����������Ƿ��л���ҹģʽ��
													// ��ֵ�ȼ�ԽС�л�Խ������ֻ�е�eDncWorkModeΪMaQue_DNC_AUTOʱ������Ч��
} MaQueDayNightCfg_s;

typedef struct
{
	XM_S32 nCompensationLevel;     	// �Զ��ع����Ȳ����ȼ���[0��100]�����ڵ����ع��Ŀ�����ȣ�ֵԽ����ͼ������Խ�ߡ�
	XM_U32 minTime;					// �Զ��ع�ʱ�����ޣ�[0��maxTime]����λ΢��
	XM_U32 maxTime; 				// �Զ��ع�ʱ�����ޣ�[0��0xFFFFFFFF]����λ΢��
	XM_U32 maxSysGainLevel;			// �Զ��ع����ϵͳ����ȼ���[0, 100]���������õ�ǰ���ϵͳ���档100��Ӧ��ϵͳ���
									// �����sensor�йء�
} MaQueAutoExpParam_s;

typedef struct
{
	MaQueManualExpLevel_e eExpLevel;	/* �ֶ��ع�ȼ��������ع�ʱ�䡣*/
} MaQueManualExpParam_s;

typedef struct
{
	MaQueOperationType_e eOpType;   	/* �ع�������ͣ���: �Զ����ֶ����֡�*/
	MaQueAutoExpParam_s stAutoExp;  	/* eOpTypeΪMAQUE_OP_TYPE_AUTOʱ��Ч��*/
	MaQueManualExpParam_s stManualExp;	/* eOpTypeΪMAQUE_OP_TYPE_MANUALʱ��Ч��*/
} MaQueExposureCfg_s;

typedef struct
{
	XM_U8	brightness;		///< ���ȣ�ȡֵ0-100��
	XM_U8	contrast;		///< �Աȶȣ�ȡֵ0-100��
	XM_U8 	saturation;		///< ���Ͷȣ�ȡֵ0-100��
	XM_U8 	hue;			///< ɫ����ȡֵ0-100��
	XM_U8 	gain;			///< ���棬ȡֵ0-100��bit7��λ��ʾ�Զ����棬����λ�����ԡ�
	XM_U8	whiteBalance;	///< �Զ��׵�ƽ���ƣ�bit7��λ��ʾ�����Զ�����.0x0,0x1,0x2�ֱ�����,��,�ߵȼ�
	XM_U16	acutance;       	///< ��ȣ�ȡֵ0-15, ��8λ��ʾˮƽ��ȣ���8Ϊ��ʾ��ֱ��ȡ�
} MaQueColorCfg_s;


typedef struct 
{
	XM_U32 bOpen;				/* ��״̬: 0: Close, 1: Open */
	MaQueLedType_e eLedType;	/* ������: 0x0: IR; 0x1: WL */
	MaQueLedMode_e eLedMode;	/* ��ģʽ: 0: Auto, 1: Manual */
} MaQueLedCfg_s;

/*
������������
	ͨ�����Դ��ڴ�ӡ������ø�Camera�Ĳ�������Ҫ���ڵ��ԡ�
��������
	  ��������			����
	pOwner	[in]	�����߱�ʶ�������ڵ��ԡ�
������ֵ��
	�ޡ�
��ע�⡿
	��API��Ҫ���ڿ����׶ε��ԡ�
*/
XM_VOID LibXmMaQue_Camera_printParam(XM_CHAR *pOwner);


/*
������������
	ע���ȡIrCut�л�ģʽ�Ļص�������
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pCallbackFuncPtr [in]	��ȡ��ǰIrCut�л�ģʽ�Ļص�������
	pCallbackArg [in]		�����ע����˽�нṹ�塣�����ڵ��ûص�����ʱ��������һ��������ֻ��ע����ʹ�ã�
							�ײ㲻���ľ������ݣ���˿�ΪNULL��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	IrCut�а���ͺ�ҹ�����˹�Ƭ���л�ʱ��ͨ����APIע��Ļص������Ƿ��л����˺�ҹ�˹�Ƭ��
*/
XM_S32 LibXmMaQue_Camera_registerIrCutCallback(XM_S32 channel, MaQueIrCutCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);	

/*
������������
	ע������ҹģʽ�л��ص���
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pCallbackFuncPtr [in]	��ȡ����ͺ�ҹ�л�ģʽ�Ļص�������
	pCallbackArg [in]		�����ע����˽�нṹ�塣�����ڵ��ûص�����ʱ��������һ��������ֻ��ע����ʹ�ã�
							�ײ㲻���ľ������ݣ���˿�ΪNULL��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	����ͷ�ڰ���ͺ�ҹ����ò�ͬ��IrCut�˹�Ƭ��Ҳ��رջ�������ƻ�׹�ƣ��л�ʱ��ͨ����APIע���
	�ص������Ƿ��л�����ҹģʽ��
*/
XM_S32 LibXmMaQue_Camera_registerDayNightCallback(XM_S32 channel, MaQueDayNightCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
������������
	���ð�ƽ��ģʽ��
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eWBMode	[in]	��ƽ��ģʽ���Զ����ֶ���ƽ�⡣Ŀǰ��ʱֻ֧���Զ���
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setWhiteBalanceMode(XM_S32 channel, MaQueWhiteBalanceMode_e eWBMode);

/*
������������
	��ȡ��ǰ��ƽ��ģʽ��
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	MaQueWhiteBalanceMode_e	��ƽ��ģʽ���У��Զ������ڣ�����ȡ�
*/
MaQueWhiteBalanceMode_e LibXmMaQue_Camera_getWhiteBalanceMode(XM_S32 channel);

/*
������������
	���ð����ҹģʽ�л����á�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstDncCfg [in]	����ͺ�ҹ�л����ò�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setDayNightCfg(XM_S32 channel, MaQueDayNightCfg_s *pstDncCfg);

/*
������������
	��ȡ�����ҹģʽ�л����á�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstDncCfg [out]		�������õİ���ͺ�ҹ�л����ò�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_getDayNightCfg(XM_S32 channel, MaQueDayNightCfg_s *pstDncCfg);

/*
������������
	�����ع����ò�����
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstExpCfg [in]	�ع����ò�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setExposureCfg(XM_S32 channel, MaQueExposureCfg_s *pstExpCfg);

/*
������������
	��ȡ�ع����ò�����
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstExpCfg [out]		�����ȡ���ع����ò�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_getExposureCfg(XM_S32 channel, MaQueExposureCfg_s *pstExpCfg);

/*
������������
	�����Ƿ����������ܡ�
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bEnabled [in]	�Ƿ����������ܡ�1��ʹ�ܣ�0���رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setAntiFlicker(XM_S32 channel, XM_S32 bEnabled);

/*
������������
	��ѯ�Ƿ����������ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	1	����ʹ�ܡ�
	0	�����رա�
*/
XM_S32 LibXmMaQue_Camera_getAntiFlicker(XM_S32 channel);

/*
������������
	���õ��������ŵȼ���
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eEsShutterLevel		���������ŵȼ���
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setEsShutterLevel(XM_S32 channel, MaQueEsShutterLevel_e eEsShutterLevel);

/*
������������
	��ȡ���������ŵȼ���
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	MaQueEsShutterLevel_e	���������ŵȼ���
*/
MaQueEsShutterLevel_e LibXmMaQue_Camera_getEsShutterLevel(XM_S32 channel);

/*
������������
	ʹ�ܻ�رձ��ⲹ����BLC����
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bEnabled [in]	�Ƿ�ʹ�ܱ��ⲹ�����ܡ�1��ʹ�ܣ�0���رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�������ⲹ������ʹ������������������ߡ��ڿ�̬ģʽ�£����ⲹ��������Ч��
*/
XM_S32 LibXmMaQue_Camera_enableBLC(XM_S32 channel, XM_S32 bEnabled);

/*
������������
	��ѯ���ⲹ�������Ƿ�����
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	1		���ⲹ��ʹ�ܡ�
	0		���ⲹ���رա�
*/
XM_S32 LibXmMaQue_Camera_isBLCEnabled(XM_S32 channel);

/*
������������
	����ͼ�����Ҿ���
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bMirror	[in]	�Ƿ�ʹ�����Ҿ���1��ʹ�ܣ�0���رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setMirror(XM_S32 channel, XM_S32 bMirror);

/*
������������
	��ѯ�Ƿ����Ҿ���ʹ�ܡ�
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	1		���Ҿ�������
	0		���Ҿ���رա�
*/
XM_S32 LibXmMaQue_Camera_getMirror(XM_S32 channel);

/*
������������
	����ͼ�����·�ת��
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bFlip  [in]		�Ƿ�ʹ�����·�ת���ܡ�1��ʹ�ܣ�0���رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setFlip(XM_S32 channel, XM_S32 bFlip);

/*
������������
	��ѯ�Ƿ����·�תʹ�ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	1		���·�תʹ�ܡ�
	0		���·�ת�رա�
*/
XM_S32 LibXmMaQue_Camera_getFlip(XM_S32 channel);

/*
������������
	����IrCut�л�����ģʽ��
��������
	  ��������				����
	channel	[in] 		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eIrCutMode [in]		IrCut�л�����ģʽ���У�������ͬ�����Զ���
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	IrCut�л�ģʽ��ֻ��ͨ��ǰ��API��LibXmMaQue_Camera_setDayNightCfg()��eDncMode����Ϊ
	MAQUE_DNC_MODE_AUTO���Զ���ʱ����Ч���������ΪMaQue_DNC_MODE_MULTICOLOR����ɫ����
	MaQue_DNC_MODE_BLACKWHITE���ڰף�ģʽ��IrCut����ǿ���л���������Ӧ��ɫ��ģʽ��
*/
XM_S32 LibXmMaQue_Camera_setIrCutMode(XM_S32 channel, MaQueIrCutMode_e eIrCutMode);

/*
������������
	��ȡIrCut�л�����ģʽ��
��������
	  ��������			����	
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	MaQueIrCutMode_e	IrCut�л�����ģʽ���У�������ͬ�����Զ��ȡ�
*/
MaQueIrCutMode_e LibXmMaQue_Camera_getIrCutMode(XM_S32 channel);

/*
������������
	����IrCut����IrCut�а���ͺ�ҹ�˹�Ƭ����ͬ�ͺſ����л���ʽ��ͬ�����޷�ͨ��������жϡ�
	Ϊ�˼��ݣ�������˱�Api������ͨ���������IrCut����
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bReversed [in]	����IrCut�Ƿ���1������0��������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_reverseIrCut(XM_S32 channel, XM_S32 bReversed);

/*
������������
	��ѯIrCut�Ƿ���
��������
	��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	1		IrCut����
	0		������
*/
XM_S32 LibXmMaQue_Camera_isIrCutReversed(XM_S32 channel);

/*
������������
	��ȡͼ�����ɫ������
��������
	��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstColorCfg	[out] ͼ�����ɫ�������ο�MaQueColorCfg_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_getColor(XM_S32 channel, MaQueColorCfg_s *pstColorCfg);

/*
������������
	����ͼ�����ɫ������
��������
	��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstColorCfg	[in] ͼ�����ɫ�������ο�MaQueColorCfg_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setColor(XM_S32 channel, MaQueColorCfg_s *pstColorCfg);

/*
������������
	��ȡ����ư����״̬��
��������
	��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	����״ֵ̬���ο�MaQueInfraredState_e ��
*/
MaQueInfraredState_e LibXmMaQue_Camera_getInfrared(XM_S32 channel);

/*
������������
	���Ƶư����ƺͰ׹�ơ�
��������
	��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstLedCfg	[in] �ƵĿ��Ʋ������ο�MaQueLedCfg_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Camera_setLed(XM_S32 channel, MaQueLedCfg_s *pstLedCfg);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_CAMERA_H


