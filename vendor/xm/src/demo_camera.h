
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_camera.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示Camera相关接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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

