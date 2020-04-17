
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_osd.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����OSD����������ݽṹ��������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
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
	MAQUE_COLOR_FMT_RGB1555,    ///< 16λ��ʾһ�����ء���ɫ����λ����ΪXRRRRRGG GGGBBBBB��
	MAQUE_COLOR_FMT_RGB565,     ///< 16λ��ʾһ�����ء���ɫ����λ����ΪRRRRRGGG GGGBBBBB��
	MAQUE_COLOR_FMT_RGB888,     ///< 24λ��ʾһ�����ء���ɫ����λ����ΪRRRRRRRR GGGGGGGG BBBBBBBB��
	MAQUE_COLOR_FMT_RGB2BPP,    ///< 2λ��ʾһ�����ء���ɫ����λ����ΪXRR GGBB��
} MaQueColorFormat_e;

typedef struct
{
	XM_S32 index;		/* ���OSD��������[0��7]��ʱ�����ռ��0������δ�̶���������ʹ�á��ײ����ֻ֧��ͬʱ����8��OSD��
						 * �粻����LibXmMaQue_OSD_showTime()��������0��Ҳ����ʹ�á�
						 */
	XM_S32 left; 		/* ȡֵ[0, 100]��Ҫ���Ӷ���������ʼλ����Ա���ͼƬ��İٷֱȡ�*/
	XM_S32 top; 		/* ȡֵ[0, 100]��Ҫ���Ӷ�����ϱ���ʼλ����Ա���ͼƬ�ߵİٷֱȡ�*/
	MaQueFontSize_e eFontSize; 	/* �ֵ����С����λ: ���ء�*/
	XM_S32 fgTrans; 	/* ǰ��͸���ȣ�[0��100]��0��ʾ��ȫ͸����*/
	XM_S32 bgTrans;		/* ����͸���ȣ�[0��100]��0��ʾ��ȫ͸����*/
	XM_S32 fgColor; 	/* ǰ����ɫ��0xAARRGGBB��ʽ��*/
	XM_S32 bgColor; 	/* ������ɫ��0xAARRGGBB��ʽ��*/
	XM_CHAR aReserved[4]; /* ������*/
} MaQueOsdParam_s;

typedef struct 
{
	XM_S32	index; /* ���OSD��������[0��7]��ʱ�����ռ��0������ռ��1������δ�̶���������ʹ�á��ײ����
					* ֻ֧��ͬʱ����8��OSD���粻����LibXmMaQue_OSD_showTime()��������0��Ҳ����ʹ�á�
					*/
	XM_S32	enable;  /* �Ƿ���ӡ�0�������ӣ�����Ĳ��������ԣ�1�����ӡ�*/

	XM_U16	x;		/* ���Ͻ�x���꣬�������������ϵ(CIF��352 x 288(PAL��)�� 240(NTSC��))��ע��:���ǿ�İٷֱȡ�*/

	XM_U16	y;      /* ���Ͻ�y���꣬�������������ϵ(CIF��352 x 288(PAL��)�� 240(NTSC��))��ע��:���Ǹߵİٷֱȡ�*/

	XM_U16	width;	/* �����OSD��Ŀ�ȣ������ص�ĸ���������ʵ�ʿ�������� CIF����ϵ��*/	

	XM_U16	height;	 /* �����OSD��ĸ߶ȣ������ص�ĸ���������ʵ�ʸߣ�������� CIF����ϵ��*/

	XM_U32	fg_color; /* ǰ����ɫ��16���Ʊ�ʾΪ0xAARRGGBB��A(Alpha)��ʾ͸���ȡ�*/
 
	XM_U32	bg_color;  /* ������ɫ��16���Ʊ�ʾΪ0xAARRGGBB��A(Alpha)��ʾ͸���ȡ�*/

	MaQueColorFormat_e eColorFormat;  /* ��ɫ��ʽ��*/

	XM_U8 *pPixel;   /* ���ӵ�������ɫֵ����� eColorFormat ��ֵΪMAQUE_COLOR_FMT_RGB1555��
					  * ����Ҫ�����ֽ�����ʾһ�����ص���ɫ������pixel�������������Ӧ���� 
					  * (width * height * 2)�ֽڡ����pixel��NULL����ʾ������ӦOSD��ĵ��ӡ�
					  */
}MaQueOsdTitleParam_s;

/*
������������
	����ʱ�䵽��Ƶͼ���С�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ���ӵ���Ƶ��ͨ����
	pstOsdParam	[in]	Osd���Ӳ�����ָ�������С����ɫ�ȡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/ 
XM_S32 LibXmMaQue_OSD_showTime(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdParam_s *pstOsdParam);

/*
������������
	������ʱ�䵽��Ƶͼ���С�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		ȡ��ʱ����ӵ���Ƶ��ͨ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_hideTime(XM_S32 channel, MaQueStreamChannel_e eStreamChn);


/*
������������
	OSD���ӳ�ʼ����
��������
	  ��������			����
	pFontFile [in]	�ֿ��ļ�·������������ʱ����Ҫ���ֿ�����ȡ����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_init(const XM_CHAR *pFontFile);


/*
������������
	���ӱ��⵽��Ƶͼ���С�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ���ӵ���Ƶ��ͨ����
	pstOsdParam	[in]	Osd���Ӳ�����
	pTitle [in]			ָ����Ҫ���ӵı����ַ��������ȡ�����ӣ��Ѹñ�������ΪNULL���ɡ�
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_setTitle(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdParam_s *pstOsdParam, XM_CHAR *pTitle);

/*
������������
	����OSD��������
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_destroy(XM_VOID);

/*
������������
	����RGB��ʽ���ص�����Ƶͼ���С�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ���ӵ���Ƶ��ͨ����
	pstParam [in]		���ӵ�RGB������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_setRGB(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueOsdTitleParam_s *pstParam);

/*
������������
	����Logo����Ƶͼ���С�
��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in]		��Ҫ���ӵ���Ƶ��ͨ����
	osdIndex [in]		ȡֵ[0��7]����Osd�������֧��8�顣����Ѿ�������LibXmMaQue_OSD_showTime()����0��ռ�ã�������ʹ�á�
	pLogoPath [in]		����ӵ�Logo�ļ�·����Ŀǰ֧��Logo�ļ�ΪJpegͼƬ��
	ePos [in]			���ӵ�λ�á�
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_OSD_setLogo(XM_S32 channel, MaQueStreamChannel_e eStreamChn, 
			XM_S32 osdIndex, XM_CHAR *pLogoPath, MaQuePosition_e ePos);


#ifdef __cplusplus
}
#endif


#endif //MAQUE_OSD_H


