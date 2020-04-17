
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_jpeg_dec.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����Jpeg����Ȳ������������ڲ�ʹ�á�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_JPEG_DECODE_H
#define MAQUE_JPEG_DECODE_H

typedef enum
{  
	RGB_COLOR_FMT_RGB555,    ///< 16λ��ʾһ�����ء���ɫ����λ����ΪXRRRRRGG GGGBBBBB��
	RGB_COLOR_FMT_RGB565,    ///< 16λ��ʾһ�����ء���ɫ����λ����ΪRRRRRGGG GGGBBBBB��
	RGB_COLOR_FMT_RGB888,    ///< 24λ��ʾһ�����ء���ɫ����λ����ΪRRRRRRRR GGGGGGGG BBBBBBBB��
} RGBColorFormat_e;


typedef struct
{                                                                                
	XM_S32 width;          ///< ͼ����                            
	XM_S32 height;         ///< ͼ��߶�                            
	XM_S32 format;         ///< ��ɫ��ʽ��ȡgraphics_color_format_t����ֵ��ֵ��
	XM_S32 pitch;          ///< ͼ����                            
	XM_S32 reference;      ///< ͼ�����ü���                        
	XM_U8 *pData;          ///< ͼ������
	XM_S32  l,t,r,b;       ///< ����ͼƬ��������,l==r ����t==b��ʾ��ͼƬ��������
	XM_U32 aResv[28-6];
} MaQueBitmap_s;

MaQueBitmap_s *Local_JpegDec_loadImage(const XM_CHAR *pFilepath);
XM_S32 Local_JpegDec_freeBitmap(MaQueBitmap_s *pstBitmap);

#endif //MAQUE_JPEG_DECODE_H


