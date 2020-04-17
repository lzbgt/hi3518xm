
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_video_out.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义视频输出相关数据结构，声明相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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
【功能描述】
	创建视频输出通道。
【参数】
	  参数名称					描述
	pstVoParam	[in]	创建视频输出所需参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoOut_create(MaQueVideoOutParam_s *pstVoParam);

/*
【功能描述】
	销毁视频输出通道。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_VideoOut_destroy(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif 


