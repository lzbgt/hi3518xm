/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_rtsp.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾRtspʵʱ��ý�岥�š�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_RTSP_H
#define MAQUE_DEMO_RTSP_H

typedef struct
{
	XM_S32 bIREnabled;
	XM_S32 bWLEnabled;
} IRWLModParam_s;

XM_S32 MaQue_Demo_Rtsp_start(XM_VOID);

#endif

