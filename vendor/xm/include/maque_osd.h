
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_osd.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义OSD叠加相关数据结构，声明相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_OSD_H
#define MAQUE_OSD_H


#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	MAQUE_FONT_SIZE_16,
	MAQUE_FONT_SIZE_24,
	MAQUE_FONT_SIZE_32,
	MAQUE_FONT_SIZE_48
} MaQueFontSize_e;

typedef enum
{  
	MAQUE_COLOR_FMT_RGB1555,    ///< 16位表示一个象素。颜色分量位掩码为XRRRRRGG GGGBBBBB。
	MAQUE_COLOR_FMT_RGB565,     ///< 16位表示一个象素。颜色分量位掩码为RRRRRGGG GGGBBBBB。
	MAQUE_COLOR_FMT_RGB888,     ///< 24位表示一个象素。颜色分量位掩码为RRRRRRRR GGGGGGGG BBBBBBBB。
	MAQUE_COLOR_FMT_RGB2BPP,    ///< 2位表示一个象素。颜色分量位掩码为XRR GGBB。
} MaQueColorFormat_e;

typedef struct
{
	XM_S32 index;		/* 标记OSD块索引，[0，7]。时间标题占用0，其它未固定，可任意使用。底层最多只支持同时叠加8块OSD。
						 * 如不调用LibXmMaQue_OSD_showTime()，则索引0，也可以使用。
						 */
	XM_S32 left; 		/* 取值[0, 100]，要叠加对象的左边起始位置相对编码图片宽的百分比。*/
	XM_S32 top; 		/* 取值[0, 100]，要叠加对象的上边起始位置相对编码图片高的百分比。*/
	MaQueFontSize_e eFontSize; 	/* 字点阵大小。单位: 像素。*/
	XM_S32 fgTrans; 	/* 前景透明度，[0，100]。0表示完全透明。*/
	XM_S32 bgTrans;		/* 背景透明度，[0，100]。0表示完全透明。*/
	XM_S32 fgColor; 	/* 前景颜色，0xAARRGGBB格式。*/
	XM_S32 bgColor; 	/* 背景颜色，0xAARRGGBB格式。*/
	XM_CHAR aReserved[4]; /* 保留。*/
} MaQueOsdParam_s;

typedef struct 
{
	XM_S32	index; /* 标记OSD块索引，[0，7]。时间标题占用0，标题占用1，其它未固定，可任意使用。底层最多
					* 只支持同时叠加8块OSD。如不调用LibXmMaQue_OSD_showTime()，则索引0，也可以使用。
					*/
	XM_S32	enable;  /* 是否叠加。0：不叠加，下面的参数将忽略，1：叠加。*/

	XM_U16	x;		/* 左上角x坐标，采用相对坐标体系(CIF：352 x 288(PAL制)或 240(NTSC制))。注意:不是宽的百分比。*/

	XM_U16	y;      /* 左上角y坐标，采用相对坐标体系(CIF：352 x 288(PAL制)或 240(NTSC制))。注意:不是高的百分比。*/

	XM_U16	width;	/* 需叠加OSD块的宽度，即像素点的个数。它是实际宽，不是相对 CIF坐标系。*/	

	XM_U16	height;	 /* 需叠加OSD块的高度，即像素点的个数。它是实际高，不是相对 CIF坐标系。*/

	XM_U32	fg_color; /* 前景颜色，16进制表示为0xAARRGGBB，A(Alpha)表示透明度。*/
 
	XM_U32	bg_color;  /* 背景颜色，16进制表示为0xAARRGGBB，A(Alpha)表示透明度。*/

	MaQueColorFormat_e eColorFormat;  /* 颜色格式。*/

	XM_U8 *pPixel;   /* 叠加的像素颜色值，如果 eColorFormat 的值为MAQUE_COLOR_FMT_RGB1555，
					  * 则需要两个字节来表示一个像素的颜色，所以pixel保存的数据至少应该是 
					  * (width * height * 2)字节。如果pixel是NULL，表示隐藏相应OSD块的叠加。
					  */
}MaQueOsdTitleParam_s;

/*
【功能描述】
	叠加时间到视频图像中。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要叠加的视频流通道。
	pstOsdParam	[in]	Osd叠加参数。指明字体大小，颜色等。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/ 
XM_S32 LibXmMaQue_OSD_showTime(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdParam_s *pstOsdParam);

/*
【功能描述】
	不叠加时间到视频图像中。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		取消时间叠加的视频流通道。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_hideTime(XM_S32 channel, MaQueStreamChannel_e eStreamChn);


/*
【功能描述】
	OSD叠加初始化。
【参数】
	  参数名称			描述
	pFontFile [in]	字库文件路径。叠加文字时，需要从字库中提取点阵。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_init(const XM_CHAR *pFontFile);


/*
【功能描述】
	叠加标题到视频图像中。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要叠加的视频流通道。
	pstOsdParam	[in]	Osd叠加参数。
	pTitle [in]			指向需要叠加的标题字符串。如果取消叠加，把该变量设置为NULL即可。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_setTitle(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdParam_s *pstOsdParam, XM_CHAR *pTitle);

/*
【功能描述】
	销毁OSD叠加任务。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_destroy(XM_VOID);

/*
【功能描述】
	叠加RGB格式像素点阵到视频图像中。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要叠加的视频流通道。
	pstParam [in]		叠加的RGB参数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_setRGB(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdTitleParam_s *pstParam);

/*
【功能描述】
	叠加Logo到视频图像中。
【参数】
	  参数名称				描述
	channel	[in]		对应sensor输入通道，从0开始。如果设备只有一个sensor该值应设置为0。
	eStreamChn [in]		需要叠加的视频流通道。
	osdIndex [in]		取值[0，7]，即Osd叠加最多支持8块。如果已经调用了LibXmMaQue_OSD_showTime()，则0被占用，不能再使用。
	pLogoPath [in]		需叠加的Logo文件路径。目前支持Logo文件为Jpeg图片。
	ePos [in]			叠加的位置。
【返回值】
	XM_SUCCESS			成功。
	其它				失败，请参考错误码。
*/
XM_S32 LibXmMaQue_OSD_setLogo(XM_S32 channel, MaQueStreamChannel_e eStreamChn, 
			XM_S32 osdIndex, XM_CHAR *pLogoPath, MaQuePosition_e ePos);


#ifdef __cplusplus
}
#endif


#endif //MAQUE_OSD_H


