
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_audio.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示录音和音频播放功能。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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

