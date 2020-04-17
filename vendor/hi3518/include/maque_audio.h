
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_audio.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义Xm MaQue 音频相关数据结构；声明对外接口。
 *		提供了录音、音频回放，编解码及输出格式设置，麦克风和扬声器音量设置或获取等。	
 *	录音流程：
 *		通过LibXmMaQue_AudioEnc_create()设置音频采集模块的输出格式，然后创建音频编码器，
 *		接收音频PCM数据开始编码。编码产生的音频帧通过该API注册的回调函数返回给调用者。
 *		音频编码器创建后，可以通过LibXmMaQue_AudioEnc_setFormat()改变编码参数。最后，
 * 		通过LibXmMaQue_AudioEnc_destroy()销毁。
 *	回放流程：
 *		通过LibXmMaQue_AudioDec_create()设置音频输出模块的参数，然后创建音频解码器，等待
 *	上层通过LibXmMaQue_AudioDec_decode()传入码流解码。音频解码器创建后，可以通过
 *	LibXmMaQue_AudioDec_setFormat()改变解码参数。最后，通过LibXmMaQue_AudioDec_destroy()
 *	销毁解码器。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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


/// 音频编码类型
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
	MaQueAudioEncodeType_e eEncType;   	  ///< 编码方式
	MaQueAudioSampleRate_e eSampleRate;   ///< 采样率
	MaQueAudioSampleBits_e eSampleBits;   ///< 采样的位深
	XM_S32 bitrateKb;	   				  ///< 码流大小，kbps为单位，比如192kbps，128kbps
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
	XM_U32 reg;   // 寄存器地址
	XM_U32 value; //寄存器的值
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
	XM_S32 bVqeEnable;	/* Vqe 总开关*/
	XM_S32 bAecEnable;	/* 回声消除*/
	XM_S32 bAnrEnable;	/* 降噪*/
	XM_S32 bAgcEnable;	/* 自动增益*/
	XM_S32 aReserved[8];
} MaQueVqeCfg_s;

/* 音频码流帧信息。*/
typedef struct 
{
	XM_U8 *pData;     /* 指向保存音频编码码流数据的指针。*/
	XM_U32 nDataLen;  /* pData中音频码流数据长度，单位为：字节。*/
	MaQueTimeStamp_s stTimeStamp;  /* 音频编码码流获取时系统时间。*/
	XM_U64 pts;	     /* 音频编码码流获取时相对1970年1月1日的时间，单位为：us。*/
	XM_U8 aHeader[8];	/* 保留，供厂家内部调试用。*/
	XM_U8 aReserved[8];	  /* 保留，用作以后扩展。*/
} MaQueAudioFrameInfo_s;

typedef XM_S32(*MaQueAudioGetStreamCallbackFuncPtr) (XM_VOID *pUserArg, MaQueAudioFrameInfo_s *pstFrameInfo);


 /*
【功能描述】
	使能和设置麦克音量。
【参数】
	 参数名称		描述									
	enable [in]	 麦克使能标识。1：使能；0：关闭。				
	volume [in]	 麦克音量。
					bit[31:16]: 保留；
					bit[15:8]: 数字增益[0，0x7F] 对应30dB到-97dB；
					bit[7:0]: 模拟增益[0,50] 对应0到50dB。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Audio_setMicVolume(XM_S32 enable, XM_S32 volume);

/*
【功能描述】
	获取麦克音量。
【参数】
	 参数名称			描述	
	pEnable	[out]	保存麦克使能标识。1：使能；0：关闭。
	pVolume	[out]	保存麦克音量。
						bit[31:16]: 保留；
						bit[15:8]: 数字增益[0，0x7F] 对应6dB到-97dB；
						bit[7:0]: 模拟增益[0,50] 对应0到50dB。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Audio_getMicVolume(XM_S32 *pEnable, XM_S32 *pVolume);

/*
【功能描述】
	使能和设置扬声器音量。
【参数】
	 参数名称		描述	
	enable [in]	 使能扬声器。
	volume [in]  扬声器音量：
					bit[31:16]: 保留；
					bit[7:0]: 增益[0，0x7F] 对应6dB到-121dB。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Audio_setSpeakerVolume(XM_S32 enable,XM_S32 volume);

/*
【功能描述】
	获取扬声器音量。
【参数】
	  参数名称		 	描述	
	pEnable	[out] 	保存扬声器使能标识。1：使能；0：关闭。
	pVolume	[out]	保存扬声器音量。
						bit[31:16]: 保留；
						bit[7:0]: 数字增益[0，0x7F] 对应6dB到-121dB。	输出
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码
*/
XM_S32 LibXmMaQue_Audio_getSpeakerVolume(XM_S32 *pEnable, XM_S32 *pVolume);

/*
【功能描述】
	创建音频编码器。
【参数】
	  参数名称			描述
	channel		[in]	音频编码器通道号，从0开始。目前暂时只支持0。
	pstFormat	[in]	创建音频编码器的参数。
	pCallbackFuncPtr [in]	调用者传入的获取音频编码码流的回调函数。
	pCallbackArg [in]	调用者传入的私有数据，将会传给回调函数的第一个参数，可在回调函数中使用。
【返回值】
	XM_SUCCESS	成功。
	其它	失败，请参考错误码。
【注意】
	目前pstFormat中,
	eEncType只支持：
		MAQUE_AUDIO_ENCODE_G711_ALAW和MAQUE_AUDIO_ENCODE_G711_ULAW。
    eSampleRate：只支持MAQUE_AUDIO_SAMPLE_RATE_8000。
*/
XM_S32 LibXmMaQue_AudioEnc_create(XM_S32 channel, MaQueAudioEncodeFormat_s *pstFormat, 
			MaQueAudioGetStreamCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*
【功能描述】
	设置音频编码器参数。创建编码器后，在编码过程中，可以随时改变编码参数。
【参数】
	  参数名称			描述	
	channel	[in]	音频编码器通道号，从0开始。目前暂时只支持0。
	pstFormat [in]	设置音频编码器的参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	LibXmMaQue_AudioEnc_create()调用成功后，再调用本API才有效。
*/
XM_S32 LibXmMaQue_AudioEnc_setFormat(XM_S32 channel, MaQueAudioEncodeFormat_s *pstFormat);

/*
【功能描述】
	销毁音频编码器，释放其分配的所有资源。
【参数】
	  参数名称			描述
	channel	[in]	音频编码器通道号，从0开始。目前暂时只支持0。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioEnc_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioEnc_destroy(XM_S32 channel);

/*
【功能描述】
	创建音频解码器。

【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pstFormat [in]	创建音频解码器的参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	目前pstFormat中eEncType只支持：
		MAQUE_AUDIO_ENCODE_G711_ALAW和MAQUE_AUDIO_ENCODE_G711_ULAW。
*/
XM_S32 LibXmMaQue_AudioDec_create(XM_S32 channel, MaQueAudioDecodeFormat_s *pstFormat);

/*
【功能描述】
	设置音频解码器参数。解码器创建成功后，在解码过程中，可以随时改变解码参数。

【参数】
	  参数名称		描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pstFormat [in]	设置音频编码器的参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。在解码过程中，
	随时改变解码参数，是允许的，但频繁修改参数会影响编码效率。
*/
XM_S32 LibXmMaQue_AudioDec_setFormat(XM_S32 channel, MaQueAudioDecodeFormat_s *pstFormat);

/*
【功能描述】
	给音频解码器送入需要解码的音频码流数据。

【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pBuffer	[in]	存有需要解码的音频码流数据。
	nLen	[in]	pBuffer中存放的音频码流数据长度。单位为：字节。强烈建议长度按帧对齐，
					这样可减少中间复制过程，提交效率。一般g711a或g711u每帧80个采样，位深
					2字节，即160字节，故最好按160整数倍传入。
【返回值】
	XM_SUCCESS		成功。
	XM_AGAIN		底层buffer已满，建议延时几毫秒后，重新传入。
	其它			失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioDec_decode(XM_S32 channel, XM_U8 *pBuffer, XM_S32 nLen);

/*
【功能描述】
	获取音频解码器缓存队列总的仍没有解码的数据长度。主要用于结束音频解码时，判断解码器是否真正解
	码完成。要判断音频输出通道是否播放完成，可用下面将要介绍的"LibXmMaQue_AudioOut_getChnStat()"。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
【返回值】
	>= 0		音频解码器缓存队列总的仍没有解码的数据长度。
	其它		失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioDec_getBufferedLen(XM_S32 channel);

/*
【功能描述】
	销毁音频解码器，释放其分配的所有资源。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
【返回值】
	XM_SUCCESS	成功。
	其它	失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioDec_destroy(XM_S32 channel);

/*
【功能描述】
	获取音频输出通道的状态信息。可查询音频输出通道占用缓冲块数是否为0，判断是否当前解码输出完成。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pstStatus [out]	音频输出通道的状态信息。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioOut_getChnStat(XM_S32 channel, MaQueAoChnState_s *pstStatus);

/*
【功能描述】
	发送PCM数据到音频输出通道播放。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pData [in]		PCM音频数据。	输入
	size [in]		pData所指的有效数据长度。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	在LibXmMaQue_AudioDec_create()调用成功后，再调用才有效。
*/
XM_S32 LibXmMaQue_AudioOut_sendPcmData(XM_S32 channel, XM_U8 *pData, XM_S32 size);

/*
【功能描述】
	设置音频输入VQE属性开关。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pstMaQueVqeCfg [in] VQE 开关配置，参考: MaQueVqeCfg_s
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	LibXmMaQue_AudioEnc_create()调用成功后，再调用本API才有效。
*/
XM_S32 LibXmMaQue_Audio_setAiVqeAttr(XM_S32 channel, MaQueVqeCfg_s *pstMaQueVqeCfg);

/*
【功能描述】
	获取音频输入VQE属性开关。
【参数】
	  参数名称			描述
	channel	[in]	音频解码器通道号，从0开始。目前暂时只支持0。
	pstMaQueVqeCfg [out] VQE 开关配置，参考: MaQueVqeCfg_s
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	LibXmMaQue_AudioEnc_create()调用成功后，再调用本API才有效。
*/
XM_S32 LibXmMaQue_Audio_getAiVqeAttr(XM_S32 channel, MaQueVqeCfg_s *pstMaQueVqeCfg);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_AUDIO_H

