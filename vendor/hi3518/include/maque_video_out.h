
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_video_out.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ������Ƶ���������ݽṹ��������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_VIDEO_OUT_H
#define MAQUE_VIDEO_OUT_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	MAQUE_VO_INTF_CVBS,
	MAQUE_VO_INTF_BT656,
	MAQUE_VO_INTF_BT1120,
	MAQUE_VO_INTF_HDMI,
	MAQUE_VO_INTF_MIPI,
	MAQUE_VO_INTF_LCD,
} MaQueVoIntfType_e;

typedef enum
{
	MAQUE_VO_BIND_CHN_MAIN,
	MAQUE_VO_BIND_CHN_2END,
} MaQueVoBindChn_e;

typedef enum
{
	MAQUE_VO_BIND_GRP_MAIN,
	MAQUE_VO_BIND_GRP_2END,
} MaQueVoBindGrp_e;


typedef struct
{
	MaQueVideoStandard_e eVidStd;
	MaQueVoIntfType_e eVoIntfType;
	MaQueVoBindGrp_e eVoBindGrp;
	MaQueVoBindChn_e eVoBindChn;
} MaQueVideoOutParam_s;


/*
������������
	������Ƶ���ͨ����
��������
	  ��������					����
	pstVoParam	[in]	������Ƶ������������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoOut_create(MaQueVideoOutParam_s *pstVoParam);

/*
������������
	������Ƶ���ͨ����
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoOut_destroy(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif 


