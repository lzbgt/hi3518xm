
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_jpeg_dec.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明Jpeg解码等操作，仅供库内部使用。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_JPEG_DECODE_H
#define MAQUE_JPEG_DECODE_H

typedef enum
{  
	RGB_COLOR_FMT_RGB555,    ///< 16位表示一个象素。颜色分量位掩码为XRRRRRGG GGGBBBBB。
	RGB_COLOR_FMT_RGB565,    ///< 16位表示一个象素。颜色分量位掩码为RRRRRGGG GGGBBBBB。
	RGB_COLOR_FMT_RGB888,    ///< 24位表示一个象素。颜色分量位掩码为RRRRRRRR GGGGGGGG BBBBBBBB。
} RGBColorFormat_e;


typedef struct
{                                                                                
	XM_S32 width;          ///< 图像宽度                            
	XM_S32 height;         ///< 图像高度                            
	XM_S32 format;         ///< 颜色格式，取graphics_color_format_t类型值的值。
	XM_S32 pitch;          ///< 图像跨度                            
	XM_S32 reference;      ///< 图像引用计数                        
	XM_U8 *pData;          ///< 图像数据
	XM_S32  l,t,r,b;       ///< 缩放图片缩放区域,l==r 或者t==b表示该图片不可缩放
	XM_U32 aResv[28-6];
} MaQueBitmap_s;

MaQueBitmap_s *Local_JpegDec_loadImage(const XM_CHAR *pFilepath);
XM_S32 Local_JpegDec_freeBitmap(MaQueBitmap_s *pstBitmap);

#endif //MAQUE_JPEG_DECODE_H


