
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_system.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义系统启动和升级相关数据结构，声明相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_SYSTEM_H
#define MAQUE_SYSTEM_H


#include "maque_struct.h"
#include "maque_video_enc.h"


#ifdef __cplusplus
extern "C" {
#endif


/* 系统启动依赖的参数。这些参数不能动态修改，修改必须重启系统，因此放在系统初始化时设置。*/
typedef struct
{
	MaQueVideoStandard_e eVidStd;  /* 视频制式，如：PAL制， NTSC制。*/
	MaQueVideoCompress_e aeVidComp[2];  /* 视频压缩标准，如：H264，H265等。aeVidComp[0]和aeVidComp[1]分别对应主辅码流。*/
	XM_CHAR aWritableDir[64];  /* 可写分区路径，用于永久保存参数。最后必需带'/'，即，必需是目录，形如：/mnt/mtd/Config/ */
} MaQueStartParam_s;

/*
【功能描述】
	初始化系统启动需要的资源，是启动App最先需要调用的函数。调用该API后，摄像头等设备
	已经初始化好，接下来就可以创建编码器。
【参数】
	  参数名称				描述
	pstStartParam [in]	系统启动过程中使用的参数。考虑到资源限制，有些模块必须在系统
						启动过程中创建起来，免得采用默认参数初始化系统后，不是实际需
						要的，还要销毁再创建。像视频编码通道，如果系统内存有限，销毁
						后，造成内存碎片，再创建可能会失败。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_System_startUp(MaQueStartParam_s *pstStartParam);

/*
【功能描述】
	初始化升级功能需要的资源，区别于LibXmMaQue_System_startUp，该接口主要用于仅实现升级业务，包括看门狗、指示灯
	和Flash 操作，编译出来的app 镜像会比全功能的小。
【参数】
	  参数名称				描述
	pstStartParam [in]	系统启动过程中使用的参数。考虑到资源限制，有些模块必须在系统
						启动过程中创建起来，免得采用默认参数初始化系统后，不是实际需
						要的，还要销毁再创建。像视频编码通道，如果系统内存有限，销毁
						后，造成内存碎片，再创建可能会失败。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_System_startUpForUpgrade(MaQueStartParam_s *pstStartParam);

/*
【功能描述】
	为了给升级提供更多内存，销毁编解码等任务，释放其资源，同时停止一切访问Flash操作。
	这个API是升级操作前，必须调用的。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。	
【注意】
	升级操作前，必须调用本API，否则升级可能会失败。
*/
XM_VOID LibXmMaQue_System_prepareForUpgrade(XM_VOID);

/*
【功能描述】
	销毁系统资源。一般IPC的App启动后，一直运行不能退出（除非异常），因此，也没有必要
	销毁。但是，在调试时，经常需要输入ctrl+c主动杀死App，然后再运行。这时，就有必要
	调用本API，彻底销毁系统资源，为再次运行App腾出资源。
【参数】
	无。
【返回值】
	无。
*/
XM_VOID LibXmMaQue_System_destroy(XM_VOID);

XM_VOID LibXmMaQue_System_reboot(XM_VOID);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_SYSTEM_H

