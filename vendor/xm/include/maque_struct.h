
/************************************************* 
 * Copyright ? Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_struct.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义常用的数据结构。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_STRUCT_H
#define MAQUE_STRUCT_H

typedef enum   /* 视频制式类型*/
{
	MAQUE_VIDEO_STANDARD_PAL = 0,	///< PAL制式。
	MAQUE_VIDEO_STANDARD_NTSC,	///< NTSC制式。
	MAQUE_VIDEO_STANDARD_SECAM,	///< SECAM制式。
	MAQUE_VIDEO_STANDARD_NR
} MaQueVideoStandard_e;

/// capture channel type
typedef enum 
{
	MAQUE_STREAM_CHN_MAIN = 0,		///< main stream 1
	MAQUE_STREAM_CHN_2END = 1,		///< extra stream 2
	MAQUE_STREAM_CHN_3IRD = 2,		///< extra stream 3
	MAQUE_STREAM_CHN_4RTH = 3,		///< extra stream 4
	MAQUE_STREAM_CHN_JPEG = 4,		///< JPEG stream
	MAQUE_STREAM_CHN_NR
} MaQueStreamChannel_e;

typedef enum
{
	MAQUE_POS_LEFT_TOP,
	MAQUE_POS_LEFT_BOTTOM,
	MAQUE_POS_LEFT_RIGHT_TOP,
	MAQUE_POS_LEFT_RIGHT_BOTTOM,
	MAQUE_POS_LEFT_CENTER
} MaQuePosition_e;

typedef struct
{
	XM_U32 width;
	XM_U32 height;
} MaQueSize_s;

typedef struct
{
	XM_S32 x;
	XM_S32 y;
	XM_U32 width;
	XM_U32 height;
} MaQueRect_s;

typedef struct
{
	XM_U32 left;		///< 左上角x坐标。
	XM_U32 top;			///< 左上角y坐标。
	XM_U32 right;		///< 右下角x坐标。
	XM_U32 bottom;		///< 右下角y坐标。
} MaQueRectCoord_s;


/// 系统时间结构
typedef struct
{
	XM_S32  year;		///< 年。   
	XM_S32  month;		///< 月，January = 1, February = 2, and so on.   
	XM_S32  day;		///< 日。   
	XM_S32  wday;		///< 星期，Sunday = 0, Monday = 1, and so on   
	XM_S32  hour;		///< 时。   
	XM_S32  minute;		///< 分。   
	XM_S32  second;		///< 秒。   
	XM_S32  isdst;		///< 夏令时标识。   
} MaQueSystemTime_s;

/// 时间戳结构
typedef struct
{
	MaQueSystemTime_s stSysTime;
	XM_U32 mSec;
} MaQueTimeStamp_s;


#endif

