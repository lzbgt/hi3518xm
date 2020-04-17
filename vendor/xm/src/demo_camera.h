
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_camera.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾCamera��ؽӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_CAMERA_H
#define MAQUE_DEMO_CAMERA_H


typedef struct
{
	XM_S32 bEnabled;
	XM_U32 nDemoTimeSecs;
	XM_CHAR aDncMode[16];
} CameraModParam_s;

MaQueDncMode_e MaQue_Demo_DncMode_str2Enum(XM_CHAR *pName);

XM_VOID MaQue_Demo_Camera_ModParam_print(XM_CHAR *pOwner, CameraModParam_s *pstModParam);

XM_S32 MaQue_Demo_Camera_start(XM_VOID);

#endif

