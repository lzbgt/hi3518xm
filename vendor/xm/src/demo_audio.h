
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_audio.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ¼������Ƶ���Ź��ܡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_AUDIO_H
#define MAQUE_DEMO_AUDIO_H


typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bWriteFile;
	XM_S32 bClosePrint;
	XM_U32 nDemoTimeSecs;	
} AudioRecordModParam_s;

XM_S32 MaQue_Demo_Audio_start(XM_VOID);

#endif

