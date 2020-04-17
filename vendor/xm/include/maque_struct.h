
/************************************************* 
 * Copyright ? Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_struct.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ���峣�õ����ݽṹ��
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_STRUCT_H
#define MAQUE_STRUCT_H

typedef enum   /* ��Ƶ��ʽ����*/
{
	MAQUE_VIDEO_STANDARD_PAL = 0,	///< PAL��ʽ��
	MAQUE_VIDEO_STANDARD_NTSC,	///< NTSC��ʽ��
	MAQUE_VIDEO_STANDARD_SECAM,	///< SECAM��ʽ��
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
	XM_U32 left;		///< ���Ͻ�x���ꡣ
	XM_U32 top;			///< ���Ͻ�y���ꡣ
	XM_U32 right;		///< ���½�x���ꡣ
	XM_U32 bottom;		///< ���½�y���ꡣ
} MaQueRectCoord_s;


/// ϵͳʱ��ṹ
typedef struct
{
	XM_S32  year;		///< �ꡣ   
	XM_S32  month;		///< �£�January = 1, February = 2, and so on.   
	XM_S32  day;		///< �ա�   
	XM_S32  wday;		///< ���ڣ�Sunday = 0, Monday = 1, and so on   
	XM_S32  hour;		///< ʱ��   
	XM_S32  minute;		///< �֡�   
	XM_S32  second;		///< �롣   
	XM_S32  isdst;		///< ����ʱ��ʶ��   
} MaQueSystemTime_s;

/// ʱ����ṹ
typedef struct
{
	MaQueSystemTime_s stSysTime;
	XM_U32 mSec;
} MaQueTimeStamp_s;


#endif

