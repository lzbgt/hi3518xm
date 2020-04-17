
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_video_encode.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ��������¼��
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_VIDEO_ENCODE_H
#define MAQUE_DEMO_VIDEO_ENCODE_H


typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bWriteFile;
	XM_S32 bClosePrint;
	XM_U32 nDemoTimeSecs;
	XM_CHAR aLogoFile[256];
	XM_CHAR aVidComp[12];
	XM_CHAR aCapSize[12];
	XM_CHAR aBitrateCtrl[12];
	XM_CHAR aImgQuality[12];
	XM_S32 nFps;
	XM_S32 nBitRate;
	XM_S32 nIFrameInterval;
	XM_S32 iQpMin;
	XM_S32 iQpMax;	
} VideoEncodeModParam_s;

typedef struct
{
	XM_S32 bEnabled;
	MaQueOsdParam_s stOsdParam;
	XM_CHAR aFontLibPath[128];
	XM_CHAR aTitle[128];
} OsdTitleModParam_s;

MaQueVideoStandard_e MaQue_Demo_VidStd_str2Enum(XM_CHAR *pName);

XM_VOID MaQue_Demo_VideoEnc_ModParam_print(XM_CHAR *pOwner, VideoEncodeModParam_s *pstVidEncModParam);
XM_VOID MaQue_Demo_VideoEnc_OsdModParam_print(XM_CHAR *pOwner, OsdTitleModParam_s *pstOsdModParam);

XM_S32 MaQue_Demo_VideoEnc_setCfg(MaQueStreamChannel_e eStreamChn, VideoEncodeModParam_s *pstModParam);
XM_S32 MaQue_Demo_VideoEnc_start(XM_VOID);

#endif

