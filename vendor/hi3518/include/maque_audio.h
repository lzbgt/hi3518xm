
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_audio.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����Xm MaQue ��Ƶ������ݽṹ����������ӿڡ�
 *		�ṩ��¼������Ƶ�طţ�����뼰�����ʽ���ã���˷���������������û��ȡ�ȡ�	
 *	¼�����̣�
 *		ͨ��LibXmMaQue_AudioEnc_create()������Ƶ�ɼ�ģ��������ʽ��Ȼ�󴴽���Ƶ��������
 *		������ƵPCM���ݿ�ʼ���롣�����������Ƶ֡ͨ����APIע��Ļص��������ظ������ߡ�
 *		��Ƶ�����������󣬿���ͨ��LibXmMaQue_AudioEnc_setFormat()�ı������������
 * 		ͨ��LibXmMaQue_AudioEnc_destroy()���١�
 *	�ط����̣�
 *		ͨ��LibXmMaQue_AudioDec_create()������Ƶ���ģ��Ĳ�����Ȼ�󴴽���Ƶ���������ȴ�
 *	�ϲ�ͨ��LibXmMaQue_AudioDec_decode()�����������롣��Ƶ�����������󣬿���ͨ��
 *	LibXmMaQue_AudioDec_setFormat()�ı������������ͨ��LibXmMaQue_AudioDec_destroy()
 *	���ٽ�������
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_AUDIO_H
#define MAQUE_AUDIO_H


#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MAQUE_AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.050K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_64000  = 64000,   /* 64K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_96000  = 96000,   /* 96K samplerate*/
    MAQUE_AUDIO_SAMPLE_RATE_BUTT,
} MaQueAudioSampleRate_e;

typedef enum
{
    MAQUE_AUDIO_SAMPLE_BITS_8   = 0,   /* 8bit width */
    MAQUE_AUDIO_SAMPLE_BITS_16  = 1,   /* 16bit width*/
    MAQUE_AUDIO_SAMPLE_BITS_24  = 2,   /* 24bit width*/
    MAQUE_AUDIO_SAMPLE_BITS_BUTT,
} MaQueAudioSampleBits_e;


/// ��Ƶ��������
typedef enum
{
	MAQUE_AUDIO_ENCODE_NONE = 0,
	MAQUE_AUDIO_ENCODE_G729_8KBIT,
	MAQUE_AUDIO_ENCODE_G726_16KBIT,
	MAQUE_AUDIO_ENCODE_G726_24KBIT,
	MAQUE_AUDIO_ENCODE_G726_32KBIT,
	MAQUE_AUDIO_ENCODE_G726_40KBIT, 
	MAQUE_AUDIO_ENCODE_PCM_8TO16BIT,
	MAQUE_AUDIO_ENCODE_PCM_ALAW,
	MAQUE_AUDIO_ENCODE_PCM_ULAW,
	MAQUE_AUDIO_ENCODE_ADPCM8K16BIT,
	MAQUE_AUDIO_ENCODE_ADPCM16K16BIT,
	MAQUE_AUDIO_ENCODE_G711_ALAW,
	MAQUE_AUDIO_ENCODE_MPEG2_LAYER1,
	MAQUE_AUDIO_ENCODE_AMR8K16BIT,
	MAQUE_AUDIO_ENCODE_G711_ULAW,
	MAQUE_AUDIO_ENCODE_IMA_ADPCM_8K16BIT,
	MAQUE_AUDIO_ENCODE_AAC_16BIT,
	MAQUE_AUDIO_ENCODE_TYPES_NR
} MaQueAudioEncodeType_e;

typedef struct 
{
	MaQueAudioEncodeType_e eEncType;   	  ///< ���뷽ʽ
	MaQueAudioSampleRate_e eSampleRate;   ///< ������
	MaQueAudioSampleBits_e eSampleBits;   ///< ������λ��
	XM_S32 bitrateKb;	   				  ///< ������С��kbpsΪ��λ������192kbps��128kbps
}MaQueAudioEncodeFormat_s;

typedef MaQueAudioEncodeFormat_s MaQueAudioDecodeFormat_s;

#if  0
typedef struct {
	XM_S32 param1;
	XM_S32 param2;
	XM_S32 param3;
} MaQueAudioTrackSensitivity_s;

typedef struct{
	XM_U32 module; // 0: codec  1:dsp
	XM_U32 reg;   // �Ĵ�����ַ
	XM_U32 value; //�Ĵ�����ֵ
} MaQueWriteRT567XReg_s;
#endif

typedef struct
{
	XM_U32 nChnTotalNum;
	XM_U32 nChnFreeNum;
	XM_U32 nChnBusyNum;
} MaQueAoChnState_s;

typedef struct
{
	XM_S32 bVqeEnable;	/* Vqe �ܿ���*/
	XM_S32 bAecEnable;	/* ��������*/
	XM_S32 bAnrEnable;	/* ����*/
	XM_S32 bAgcEnable;	/* �Զ�����*/
	XM_S32 aReserved[8];
} MaQueVqeCfg_s;

/* ��Ƶ����֡��Ϣ��*/
typedef struct 
{
	XM_U8 *pData;     /* ָ�򱣴���Ƶ�����������ݵ�ָ�롣*/
	XM_U32 nDataLen;  /* pData����Ƶ�������ݳ��ȣ���λΪ���ֽڡ�*/
	MaQueTimeStamp_s stTimeStamp;  /* ��Ƶ����������ȡʱϵͳʱ�䡣*/
	XM_U64 pts;	     /* ��Ƶ����������ȡʱ���1970��1��1�յ�ʱ�䣬��λΪ��us��*/
	XM_U8 aHeader[8];	/* �������������ڲ������á�*/
	XM_U8 aReserved[8];	  /* �����������Ժ���չ��*/
} MaQueAudioFrameInfo_s;

typedef XM_S32(*MaQueAudioGetStreamCallbackFuncPtr) (XM_VOID *pUserArg, MaQueAudioFrameInfo_s *pstFrameInfo);


 /*
������������
	ʹ�ܺ��������������
��������
	 ��������		����									
	enable [in]	 ���ʹ�ܱ�ʶ��1��ʹ�ܣ�0���رա�				
	volume [in]	 ���������
					bit[31:16]: ������
					bit[15:8]: ��������[0��0x7F] ��Ӧ30dB��-97dB��
					bit[7:0]: ģ������[0,50] ��Ӧ0��50dB��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Audio_setMicVolume(XM_S32 enable, XM_S32 volume);

/*
������������
	��ȡ���������
��������
	 ��������			����	
	pEnable	[out]	�������ʹ�ܱ�ʶ��1��ʹ�ܣ�0���رա�
	pVolume	[out]	�������������
						bit[31:16]: ������
						bit[15:8]: ��������[0��0x7F] ��Ӧ6dB��-97dB��
						bit[7:0]: ģ������[0,50] ��Ӧ0��50dB��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Audio_getMicVolume(XM_S32 *pEnable, XM_S32 *pVolume);

/*
������������
	ʹ�ܺ�����������������
��������
	 ��������		����	
	enable [in]	 ʹ����������
	volume [in]  ������������
					bit[31:16]: ������
					bit[7:0]: ����[0��0x7F] ��Ӧ6dB��-121dB��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Audio_setSpeakerVolume(XM_S32 enable,XM_S32 volume);

/*
������������
	��ȡ������������
��������
	  ��������		 	����	
	pEnable	[out] 	����������ʹ�ܱ�ʶ��1��ʹ�ܣ�0���رա�
	pVolume	[out]	����������������
						bit[31:16]: ������
						bit[7:0]: ��������[0��0x7F] ��Ӧ6dB��-121dB��	���
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο�������
*/
XM_S32 LibXmMaQue_Audio_getSpeakerVolume(XM_S32 *pEnable, XM_S32 *pVolume);

/*
������������
	������Ƶ��������
��������
	  ��������			����
	channel		[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstFormat	[in]	������Ƶ�������Ĳ�����
	pCallbackFuncPtr [in]	�����ߴ���Ļ�ȡ��Ƶ���������Ļص�������
	pCallbackArg [in]	�����ߴ����˽�����ݣ����ᴫ���ص������ĵ�һ�����������ڻص�������ʹ�á�
������ֵ��
	XM_SUCCESS	�ɹ���
	����	ʧ�ܣ���ο������롣
��ע�⡿
	ĿǰpstFormat��,
	eEncTypeֻ֧�֣�
		MAQUE_AUDIO_ENCODE_G711_ALAW��MAQUE_AUDIO_ENCODE_G711_ULAW��
    eSampleRate��ֻ֧��MAQUE_AUDIO_SAMPLE_RATE_8000��
*/
XM_S32 LibXmMaQue_AudioEnc_create(XM_S32 channel, MaQueAudioEncodeFormat_s *pstFormat, 
			MaQueAudioGetStreamCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
������������
	������Ƶ�������������������������ڱ�������У�������ʱ�ı���������
��������
	  ��������			����	
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstFormat [in]	������Ƶ�������Ĳ�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	LibXmMaQue_AudioEnc_create()���óɹ����ٵ��ñ�API����Ч��
*/
XM_S32 LibXmMaQue_AudioEnc_setFormat(XM_S32 channel, MaQueAudioEncodeFormat_s *pstFormat);

/*
������������
	������Ƶ���������ͷ�������������Դ��
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioEnc_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioEnc_destroy(XM_S32 channel);

/*
������������
	������Ƶ��������

��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstFormat [in]	������Ƶ�������Ĳ�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	ĿǰpstFormat��eEncTypeֻ֧�֣�
		MAQUE_AUDIO_ENCODE_G711_ALAW��MAQUE_AUDIO_ENCODE_G711_ULAW��
*/
XM_S32 LibXmMaQue_AudioDec_create(XM_S32 channel, MaQueAudioDecodeFormat_s *pstFormat);

/*
������������
	������Ƶ�����������������������ɹ����ڽ�������У�������ʱ�ı���������

��������
	  ��������		����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstFormat [in]	������Ƶ�������Ĳ�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч���ڽ�������У�
	��ʱ�ı���������������ģ���Ƶ���޸Ĳ�����Ӱ�����Ч�ʡ�
*/
XM_S32 LibXmMaQue_AudioDec_setFormat(XM_S32 channel, MaQueAudioDecodeFormat_s *pstFormat);

/*
������������
	����Ƶ������������Ҫ�������Ƶ�������ݡ�

��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pBuffer	[in]	������Ҫ�������Ƶ�������ݡ�
	nLen	[in]	pBuffer�д�ŵ���Ƶ�������ݳ��ȡ���λΪ���ֽڡ�ǿ�ҽ��鳤�Ȱ�֡���룬
					�����ɼ����м临�ƹ��̣��ύЧ�ʡ�һ��g711a��g711uÿ֡80��������λ��
					2�ֽڣ���160�ֽڣ�����ð�160���������롣
������ֵ��
	XM_SUCCESS		�ɹ���
	XM_AGAIN		�ײ�buffer������������ʱ����������´��롣
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioDec_decode(XM_S32 channel, XM_U8 *pBuffer, XM_S32 nLen);

/*
������������
	��ȡ��Ƶ��������������ܵ���û�н�������ݳ��ȡ���Ҫ���ڽ�����Ƶ����ʱ���жϽ������Ƿ�������
	����ɡ�Ҫ�ж���Ƶ���ͨ���Ƿ񲥷���ɣ��������潫Ҫ���ܵ�"LibXmMaQue_AudioOut_getChnStat()"��
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
������ֵ��
	>= 0		��Ƶ��������������ܵ���û�н�������ݳ��ȡ�
	����		ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioDec_getBufferedLen(XM_S32 channel);

/*
������������
	������Ƶ���������ͷ�������������Դ��
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
������ֵ��
	XM_SUCCESS	�ɹ���
	����	ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioDec_destroy(XM_S32 channel);

/*
������������
	��ȡ��Ƶ���ͨ����״̬��Ϣ���ɲ�ѯ��Ƶ���ͨ��ռ�û�������Ƿ�Ϊ0���ж��Ƿ�ǰ���������ɡ�
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstStatus [out]	��Ƶ���ͨ����״̬��Ϣ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioOut_getChnStat(XM_S32 channel, MaQueAoChnState_s *pstStatus);

/*
������������
	����PCM���ݵ���Ƶ���ͨ�����š�
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pData [in]		PCM��Ƶ���ݡ�	����
	size [in]		pData��ָ����Ч���ݳ��ȡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��LibXmMaQue_AudioDec_create()���óɹ����ٵ��ò���Ч��
*/
XM_S32 LibXmMaQue_AudioOut_sendPcmData(XM_S32 channel, XM_U8 *pData, XM_S32 size);

/*
������������
	������Ƶ����VQE���Կ��ء�
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstMaQueVqeCfg [in] VQE �������ã��ο�: MaQueVqeCfg_s
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	LibXmMaQue_AudioEnc_create()���óɹ����ٵ��ñ�API����Ч��
*/
XM_S32 LibXmMaQue_Audio_setAiVqeAttr(XM_S32 channel, MaQueVqeCfg_s *pstMaQueVqeCfg);

/*
������������
	��ȡ��Ƶ����VQE���Կ��ء�
��������
	  ��������			����
	channel	[in]	��Ƶ������ͨ���ţ���0��ʼ��Ŀǰ��ʱֻ֧��0��
	pstMaQueVqeCfg [out] VQE �������ã��ο�: MaQueVqeCfg_s
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	LibXmMaQue_AudioEnc_create()���óɹ����ٵ��ñ�API����Ч��
*/
XM_S32 LibXmMaQue_Audio_getAiVqeAttr(XM_S32 channel, MaQueVqeCfg_s *pstMaQueVqeCfg);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_AUDIO_H

