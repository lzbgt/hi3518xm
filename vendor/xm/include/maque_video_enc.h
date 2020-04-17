
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_video_enc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ������Ƶ����������ݽṹ��������ض���ӿڡ�
 *		��Ƶ������Ҫ���Ĵ����ڴ棬����ڵ���LibXmMaQue_System_startUp()ʱ�����Ѿ�����
 *		��ɣ�����ʼ���롣�����ṩ��API�����ڱ�����������ɣ�����ʼ����Ĺ����С�
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_VIDEO_ENC_H
#define MAQUE_VIDEO_ENC_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum  
{
	MAQUE_VIDEO_COMPRESS_DIVX_MPEG4,	///< DIVX MPEG4��
	MAQUE_VIDEO_COMPRESS_MS_MPEG4,		///< MS MPEG4��
	MAQUE_VIDEO_COMPRESS_MPEG2,			///< MPEG2��
	MAQUE_VIDEO_COMPRESS_MPEG1,			///< MPEG1��
	MAQUE_VIDEO_COMPRESS_H263,			///< H.263
	MAQUE_VIDEO_COMPRESS_MJPG,			///< MJPG
	MAQUE_VIDEO_COMPRESS_FCC_MPEG4,		///< FCC MPEG4
	MAQUE_VIDEO_COMPRESS_H264,			///< H.264
	MAQUE_VIDEO_COMPRESS_H265,			///< H.265
	MAQUE_VIDEO_COMPRESS_JPEG,			///< JPEG
	MAQUE_VIDEO_COMPRESS_NR				///< 
} MaQueVideoCompress_e;

/// ����ֱ�������
/// \note������������ṹʱ��Ҫͬ������Record.cpp����֤��������
typedef enum 
{
    MAQUE_CAPTURE_SIZE_D1,		///< 720*576(PAL)	720*480(NTSC)
    MAQUE_CAPTURE_SIZE_HD1,		///< 352*576(PAL)	352*480(NTSC)
    MAQUE_CAPTURE_SIZE_BCIF,	///< 720*288(PAL)	720*240(NTSC)
    MAQUE_CAPTURE_SIZE_CIF,		///< 352*288(PAL)	352*240(NTSC)
    MAQUE_CAPTURE_SIZE_QCIF,	///< 176*144(PAL)	176*120(NTSC)
    MAQUE_CAPTURE_SIZE_VGA,		///< 640*480(PAL)	640*480(NTSC)
    MAQUE_CAPTURE_SIZE_QVGA,	///< 320*240(PAL)	320*240(NTSC)
    MAQUE_CAPTURE_SIZE_360P,	///< 640*360(PAL)	640*360(NTSC)
    MAQUE_CAPTURE_SIZE_QQVGA,	///< 160*128(PAL)	160*128(NTSC)
    MAQUE_CAPTURE_SIZE_ND1 = 9,   ///< 240*192
    MAQUE_CAPTURE_SIZE_650TVL,    ///< 928*576
    MAQUE_CAPTURE_SIZE_720P,      ///< 1280*720
    MAQUE_CAPTURE_SIZE_1_3M,      ///< 1280*960
    MAQUE_CAPTURE_SIZE_UXGA ,     ///< 1600*1200
    MAQUE_CAPTURE_SIZE_1080P,     ///< 1920*1080
    MAQUE_CAPTURE_SIZE_WUXGA,     ///< 1920*1200
    MAQUE_CAPTURE_SIZE_2_5M,      ///< 1872*1408
    MAQUE_CAPTURE_SIZE_3M,        ///< 2048*1536
    MAQUE_CAPTURE_SIZE_5M,        ///< 3744*1408
    MAQUE_CAPTURE_SIZE_EXT_NR =19,    ///< ö�ٵ�ͼ�δ�С�������Ŀ��
    MAQUE_CAPTURE_SIZE_1080N = 19,    ///< 960*1080
    MAQUE_CAPTURE_SIZE_4M,        	///< 2592*1520
    MAQUE_CAPTURE_SIZE_6M,			///< 3072��2048
    MAQUE_CAPTURE_SIZE_8M,			///< 3264��2448
    MAQUE_CAPTURE_SIZE_12M,			///< 4000*3000
    MAQUE_CAPTURE_SIZE_4K,			///< 4096 * 2160ͨ��/3840*2160��˼
    MAQUE_CAPTURE_SIZE_EXT_V2_NR=25,    ///< ö�ٵ�ͼ�δ�С�������Ŀ��
    MAQUE_CAPTURE_SIZE_720N = 25,  ///  640*720
    MAQUE_CAPTURE_SIZE_WSVGA,      ///< 1024*576
    MAQUE_CAPTRUE_SIZE_RES2,       // 0*0
    MAQUE_CAPTURE_SIZE_RES3,       // 0*0
    MAQUE_CAPTURE_SIZE_RES4,       // 0*0
    MAQUE_CAPTURE_SIZE_RES5,       // 0*0
    MAQUE_CAPTURE_SIZE_RES6,       // 0*0
    MAQUE_CAPTURE_SIZE_NR,
} MaQueCaptureSize_e;

typedef enum  
{
	MAQUE_BITRATE_CTRL_CBR,			///< �㶨����
	MAQUE_BITRATE_CTRL_VBR,			///< �ɱ�����
	MAQUE_BITRATE_CTRL_MBR,			///< �������
	MAQUE_BITRATE_CTRL_NR			///< 
} MaQueBitrateCtrl_e;


/// image quality
typedef enum
{
	MAQUE_IMG_QUALITY_WORST = 1,		///< ���
	MAQUE_IMG_QUALITY_WORSE = 2,		///< �ϲ�
	MAQUE_IMG_QUALITY_GENERAL = 3,		///< һ��
	MAQUE_IMG_QUALITY_GOOD = 4,			///< ��
	MAQUE_IMG_QUALITY_BETTER = 5,		///< ����
	MAQUE_IMG_QUALITY_BEST = 6,			///< ���
	MAQUE_IMG_QUALITY_NR = 7
} MaQueImageQuality_e;

typedef enum 
{
	IFRAME_INTV_TYPE_TIME,
	IFRAME_INTV_TYPE_FRAME_COUNT,
	IFRAME_INTV_TYPE_NR,
} IFrameIntervalType_e;


typedef struct
{
	MaQueCaptureSize_e eCapSizeMax;  /* ��Ƶͨ��������ֱ��ʡ�*/
} MaQueVidChnAbility_s;

typedef struct
{
	XM_U64 videoEncTypeMask;  /* ��Ƶ���루��ѹ����׼���������룬��֧�ֵ�MaQueCaptureSize_e����ö��ֵ��Ӧ��λ��������á�
							   *  �磺֧��H265����videoEncTypeMask |= 1 << MAQUE_VIDEO_COMPRESS_H265��
							   */
	XM_U32 maxEncPowerX1080P;  /* ��������������λΪ��1080P����ÿ��֧�ֶ���֡1080P���롣*/
	MaQueCaptureSize_e eDecImageSizeMax;  /* ֧�ֵ�������ֱ��ʡ������֧����Ϊ��MAQUE_CAPTURE_SIZE_NR��*/
	MaQueVidChnAbility_s astVidEncChnAbility[MAQUE_STREAM_CHN_NR]; /* ÿ����Ƶͨ��������Jpeg����ͨ�����ı���������*/
} MaQueCodeAbilities_s;

typedef struct 
{
	MaQueVideoCompress_e	eVidComp;  /* ��Ƶѹ����׼��H264��H265�ȣ���*/
	MaQueCaptureSize_e		eCapSize;	/* ��Ƶ�ֱ��ʡ�*/
	MaQueBitrateCtrl_e		eBitrateCtrl;  /* �����������͡�*/
	MaQueImageQuality_e		eImageQuality;  /* ����ͼ��������*/
	XM_S32		nFps;					/* ����֡�ʡ���ÿ������֡��*/
	XM_S32		nBitRate;				/* ��������ʡ���λ��Kb��*/
	IFrameIntervalType_e eIFrmIntvType;  /* I֡�������ʱ���֡��������ο�: IFrameIntervalType_e */
	XM_S32		nIFrameInterval;		/* I֡�����������nIFrameInterval ��һ��I֡������I֡���֡��������ǰһ������eIFrmIntvType������*/
	XM_U32 		bUseUserQp;     /* �Ƿ�ʹ������ָ����Qpֵ��*/
	XM_S32		iQpMin;			/* ��СQPֵ��[1��51]���� bUseUserQpΪ1ʱ������Ч��ע��Qpֵ�ڿɱ����ʱ���ʱ���ã�
								 * ���ͱ��������һ�����ͼ��������ʹ��QpֵԽС������������Խ��ͼ������Խ�á�
								 */
	XM_S32		iQpMax;			/* ���QPֵ��[1��51]���� bUseUserQpΪ1ʱ������Ч��*/
} MaQueVideoEncodeCfg_s;

#if 0
typedef struct 
{
	XM_S32 nMainAudio;                // Main stream audio flag: 0 - disable 1 - enable
	XM_S32 nExtraVideo;               // Extra stream video flag: 0 - disable 1 - enable
	XM_S32 nExtraAudio;               // Extra stream audio flag: 0 - disable 1 - enable
	MaQueVideoFormat_s stMain;  // Main stream
	MaQueVideoFormat_s stExtra; // Extra stream
	MaQueVideoFormat_s stSnap;  // Snap stream
}MaQueVideoEncodeCfg_s;
#endif

typedef enum
{
	MAQUE_HEADER_TYPE_FRAME_INFO = 1,
	MAQUE_HEADER_TYPE_USER,
} MaQueHeaderType_e;

/// FrameType
typedef enum
{
	MAQUE_FRAME_TYPE_NONE,
	MAQUE_FRAME_TYPE_VIDEO,
	MAQUE_FRAME_TYPE_AUDIO,
	MAQUE_FRAME_TYPE_DATA
} MaQueFrameType_e;


/// FrameSubType
typedef enum
{
	MAQUE_FRAME_SUBTYPE_I = 0,
	MAQUE_FRAME_SUBTYPE_P = 1,
	MAQUE_FRAME_SUBTYPE_B = 2,
	MAQUE_FRAME_SUBTYPE_S = 3,
	MAQUE_FRAME_SUBTYPE_TEXT = 5,  /* Data */
	MAQUE_FRAME_SUBTYPE_TNTL = 6,
} MaQueFrameSubType_e;


/// ��������
typedef enum
{
	MAQUE_ENCODE_TYPE_NONE = 0,
	MAQUE_ENCODE_TYPE_H264 = 1,
	MAQUE_ENCODE_TYPE_H265 = 2,
	MAQUE_ENCODE_TYPE_JPEG = 3,
} MaQueEncodeType_e;

typedef struct 
{
	XM_HANDLE handleMem;	/* �ο�ǰ�棺maque_mem.h�½ڣ����� pMemAllocFuncPtr���ص�pHandle�������ϲ��ͷ��ڴ档*/
	XM_U8 *pHeader;			/* ָ����Ƶ��֡ͷ���ݡ��ݱ��������á�*/
	XM_U32 nHeaderLen;		/* ��Ƶ��֡ͷ���ݳ��ȡ���λ���ֽڡ��ݱ��������á�*/ 
	MaQueHeaderType_e eHeaderType;	/* ��Ƶ��֡ͷ���͡��ݱ��������á�*/
	XM_U8 *pData;            	/* ָ����Ƶ�������ݡ�*/
	XM_U32 nDataLen;         	/* ��Ƶ�������ݳ��ȡ�*/
	XM_U32 nMallocLen;       	/* pData��ָBuffer��С��*/
	MaQueFrameType_e eFrameType;     /* ��Ƶ������Ӧ��֡���ͣ�ָ������Ƶ����Ƶ�����ݵȣ���*/
	MaQueFrameSubType_e eSubType;    /* ��Ƶ������Ӧ��֡�����ͣ�ָ����I��P��B֡�ȣ���*/
	MaQueEncodeType_e eEncodeType;   /* ��Ƶ������Ӧ�ı������ͣ�ָ����H264��H265�ȣ���*/
	XM_U32 nFrameRate;			/* ��Ƶ֡�ʡ�*/
	XM_S32 nWidth;			    /* ��Ƶͼ���*/
	XM_S32 nHeight;             /* ��Ƶͼ��ߡ�*/
	
	MaQueTimeStamp_s stTimeStamp;  /* ʱ�����*/
	XM_U64 pts;		/* ��ʾʱ�������λ��us��*/
	XM_U32 version;		/* �������ݲ��á�*/	
	XM_U8 aReserved[16];	/* ������*/
} MaQueVideoEncFrameInfo_s;

typedef XM_S32 (*MaQueVideoGetStreamCallbackFuncPtr) (XM_VOID *pUserArg, MaQueVideoEncFrameInfo_s *pstFrameInfo);

typedef struct 
{
	MaQueRectCoord_s stRectCoord;  /* ��Ƶ�ڵ���ľ������ꡣ*/
	XM_U32 color;    /* ��Ƶ�ڵ���������ɫ��*/
	XM_U32 enable;   /* �Ƿ���ʾ��Ƶ�ڵ��顣1����ʾ��0�����ء�*/
} MaQueVideoCoverParam_s;

typedef enum
{
	MAQUE_VIDEO_ENC_PROFILE_BASE,
	MAQUE_VIDEO_ENC_PROFILE_MAIN,
	MAQUE_VIDEO_ENC_PROFILE_HIGH,
	MAQUE_VIDEO_ENC_PROFILE_NR,
} MaQueVideoEncProfile_e;


/*
������������
	ͼ��ֱ�������ת����ö��ֵ�����磺"3M"���룬������ö��ֵMAQUE_CAPTURE_SIZE_3M��
��������
	  ��������				����
	pCapSizeName [in]	ͼ��ֱ��������ַ�����
������ֵ��
	MaQueCaptureSize_e	�ֱ���ö��ֵ�����ǺϷ��ַ�����������MAQUE_CAPTURE_SIZE_NR��
��ע�⡿
	ͼ��ֱ���ö��ֵ�����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueCaptureSize_e"��
*/
MaQueCaptureSize_e LibXmMaQue_Video_CaptureSize_toEnum(XM_CHAR *pCapSizeName);

/*
������������
	ͼ��ֱ���ö��ֵת���������ַ��������磺MAQUE_CAPTURE_SIZE_3M���룬������"3M"��
��������
	  ��������			����
	eCapSize [in]	ͼ��ֱ���ö��ֵ��
������ֵ��
	�����ַ���	ͼ��ֱ���ö��ֵ��Ӧ���ַ�����
��ע�⡿
	ͼ��ֱ���ö��ֵ�����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueCaptureSize_e"��
*/
XM_CHAR *LibXmMaQue_Video_CaptureSize_toStr(MaQueCaptureSize_e eCapSize);

/*
������������
	��Ƶѹ����׼����ת����ö��ֵ�����磺"H265"���룬������ö��ֵMAQUE_VIDEO_COMPRESS_H265��
��������
	  ��������				����
	pEncTypName [in]	��Ƶѹ����׼�����ַ�����
������ֵ��
	MaQueVideoCompress_e	��Ƶѹ����׼ö��ֵ�����ǺϷ��ַ�����������MAQUE_VIDEO_COMPRESS_NR��
��ע�⡿
	��Ƶѹ����׼ö��ֵ�����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueVideoCompress_e"��
*/
MaQueVideoCompress_e LibXmMaQue_Video_Compress_toEnum(XM_CHAR *pEncTypName);

/*
������������
	��Ƶѹ����׼ö��ֵת���������ַ��������磺MAQUE_VIDEO_COMPRESS_H265���룬������"H265"��
��������
	  ��������				����	
	eVideoComp [in]		��Ƶѹ����׼ö��ֵ��
������ֵ��
	�����ַ���	��Ƶѹ����׼ö��ֵ��Ӧ���ַ�����
��ע�⡿
	��Ƶѹ����׼ö��ֵ�����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueVideoCompress_e"��
*/
XM_CHAR *LibXmMaQue_Video_Compress_toStr(MaQueVideoCompress_e eVideoComp);

/*
������������
	���ʿ�����������ת����ö��ֵ�����磺"VBR"���룬������ö��ֵMAQUE_BITRATE_CTRL_VBR��
��������
	  ��������				����
	pBitCtrlName [in]	���ʿ������������ַ�����
������ֵ��
	MaQueBitrateCtrl_e	���ʿ�������ö��ֵ�����ǺϷ��ַ�����������MAQUE_BITRATE_CTRL_NR��
��ע�⡿
	���ʿ�������ö��ֵ����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueBitrateCtrl_e"��
*/
MaQueBitrateCtrl_e LibXmMaQue_Video_BitrateCtrl_toEnum(XM_CHAR *pBitCtrlName);

/*
������������
	���ʿ�������ö��ֵת���������ַ��������磺MAQUE_BITRATE_CTRL_VBR���룬������"VBR"��
��������
	  ��������				����
	eBitrateCtrl [in]	���ʿ�������ö��ֵ��
������ֵ��
	�����ַ���	���ʿ�������ö��ֵ��Ӧ���ַ�����
��ע�⡿
	���ʿ�������ö��ֵ����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueBitrateCtrl_e"��
*/
XM_CHAR *LibXmMaQue_Video_BitrateCtrl_toStr(MaQueBitrateCtrl_e eBitrateCtrl);

/*
������������
	ͼ����������ת����ö��ֵ�����磺"Good"���룬������ö��ֵMAQUE_IMG_QUALITY_GOOD��
��������
	  ��������			����	
	pName [in]		ͼ�����������ַ�����
������ֵ��
	MaQueImageQuality_e	ͼ������ö��ֵ�����ǺϷ��ַ�����������MAQUE_IMG_QUALITY_NR��
��ע�⡿
	ͼ������ö��ֵ����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueImageQuality_e"��
*/
MaQueImageQuality_e LibXmMaQue_ImageQuality_toEnum(XM_CHAR *pName);

/*
������������
	ͼ������ö��ֵת���������ַ��������磺MAQUE_IMG_QUALITY_GOOD���룬������"Good"��
��������
	  ��������				����
	eImgQuality [in]	ͼ������ö��ֵ��
������ֵ��
	�����ַ���	ͼ������ö��ֵ��Ӧ���ַ�����
��ע�⡿
	ͼ������ö��ֵ����Ӧ�������ַ�����ϵ��ο����棺"��������>> MaQueImageQuality_e"��
*/
XM_CHAR *LibXmMaQue_ImageQuality_toStr(MaQueImageQuality_e eImgQuality);

/*
������������
	��ȡ�ײ�֧�ֵı�������������

��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��	
	pstCodeAbility	[out]	����ײ�֧�ֵı�������������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_getAbilities(XM_S32 channel, MaQueCodeAbilities_s *pstCodeAbility);

/*
������������
	ǿ��I֡�����ñ��������ϲ���I֡��
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		ǿ�Ʋ���I֡����Ƶ��ͨ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_forceIFrame(XM_S32 channel, MaQueStreamChannel_e eStreamChn);

/*
������������
	��̬������Ƶ������������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ�޸����õ���Ƶ��ͨ����
	pstEncCfg [in]		��Ƶ���������
������ֵ��
	XM_SUCCESS		�ɹ���
	1				��Ҫ�����������ڴ����ƣ�һ������ʽ�ı䣨��H264��H265���У���Ҫ�������ӡ�
	����			ʧ�ܣ���ο������롣
��ע�⡿
	ǰ��"maque_system.h"�½ڣ�LibXmMaQue_System_startUp()�Ĳ������趨��ʼ�������ͣ����ñ�APIʱ��
	ֻҪ�������ͣ��Ϳ��Ա���������ע��ʵ��Ӧ���У���Ҫ��������ǰѡ��ı������ͣ������󣬴���
	LibXmMaQue_System_startUp()��������Ҫ�ı�������
*/
XM_S32 LibXmMaQue_VideoEnc_setCfg(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueVideoEncodeCfg_s *pstEncCfg);

/*
������������
	��ȡ��ǰ��Ƶ������������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ��ȡ���õ���Ƶ��ͨ����
	pstEncCfg [out]		��Ƶ���������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_getCfg(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueVideoEncodeCfg_s *pstEncCfg);

/*
������������
	ע��ص���������ʼ��ȡ��Ƶ��������
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]			��Ҫ��ȡ��Ƶ����ͨ����
	pCallbackFuncPtr [in]	��ȡ��Ƶ�������Ļص�����ָ�롣��Ƶ���������һ֡�󣬾�ͨ���ûص������ϴ���ע���ߡ�
	pCallbackArg [in]		�����˽�нṹ�塣�ڵ��ûص�����pCallbackFuncPtrʱ��������һ��������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_startStream(XM_S32 channel, MaQueStreamChannel_e eStreamChn, 
			MaQueVideoGetStreamCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
������������
	ֹͣ��ȡ��Ƶ��������
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫֹͣ��Ƶ����ͨ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_stopStream(XM_S32 channel, MaQueStreamChannel_e eStreamChn);

/*
������������
	������Ƶ����ȼ�(Profile)��H264:֧��base��main��high�ȣ���H265��֧��main��
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	vencChnMask [in]	��Ҫ���õı���ͨ�����룬��: ����ͨ��n��vencChnMask |= (1 << n)��
	eVidEncProfile [in] ��Ҫ���õı���ȼ�(Profile)���ο�: MaQueVideoEncProfile_e��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�󲿷�ƽ̨(��: HI3516CV500��HI3516DV300��)��H265��֧��main���ײ��д������������ֵ������Ч��
*/

XM_S32 LibXmMaQue_VideoEnc_setProfile(XM_S32 channel, XM_U32 vencChnMask, MaQueVideoEncProfile_e eVidEncProfile);

/*
������������
	ʹ�ܻ�ر�H264��H265 SmartP���롣
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	vencChnMask [in]	��Ҫ���õı���ͨ�����룬��: ����ͨ��n��vencChnMask |= (1 << n)��
	bEnabled [in] 		�����Ƿ�ʹ��SmartP��1: ʹ�ܣ�0: �رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/

XM_S32 LibXmMaQue_VideoEnc_setSmartP(XM_S32 channel, XM_U32 vencChnMask, XM_S32 bEnabled);

/*
������������
	������Ƶ�����ڵ���
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	index [in]		�ڵ���������[0��3]�����֧��4���ڵ��顣
	pstParam [in]	��Ƶ�ڵ�������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_setCover(XM_S32 channel, XM_S32 index, MaQueVideoCoverParam_s *pstParam);

/*
������������
	ʹ�ܻ�ر�WDRģʽ��
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bEnabled [in] 		�����Ƿ�ʹ��WDR��1: ʹ�ܣ�0: �رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_VideoEnc_switchWdrMode(XM_S32 channel, XM_S32 bEnabled);


#ifdef __cplusplus
}
#endif

#endif 


