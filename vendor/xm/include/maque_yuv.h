
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_yuv.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����YUV��ȡ������ݽṹ��������ض���ӿڡ�
 *		��ͨ����ѯ��ע��ص���ʽ��ȡ��ע�⣬�ص��Ͳ�ѯ��ʽ����ͬʱʹ�ã�ֻ������
 *		ʹ�ܵķ�ʽ��Ч��ͨ��LibXmMaQue_YUV_create()�趨��ʼ��ͨ����С��ͨ��
 *		LibXmMaQue_YUV_setParam()�޸ġ���LibXmMaQue_YUV_getFrame()�����޸�ͨ����С��
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_YUV_H
#define MAQUE_YUV_H


#ifdef __cplusplus
extern "C" {
#endif

#define YUV_WIDTH_ALIGN			32
#define YUV_WIDTH_ALIGN_MASK	0x1F
#define YUV_HEIGHT_ALIGN		1
#define YUV_HEIGHT_ALIGN_MASK	0x1

typedef enum 
{
   MAQUE_YUV_PIXEL_FORMAT_YVU_SP420,   /* SP420��ʽ���ȱ�������Y���ݣ���VU����洢��VU��Y�Ŀ����ͬ���߶�ֻ��Y��һ�롣*/
   MAQUE_YUV_PIXEL_FORMAT_YUV_SP420,   /* SP420��ʽ���ȱ�������Y���ݣ���UV����洢��UV��Y�Ŀ����ͬ���߶�ֻ��Y��һ�롣*/
} MaQueYuvPixelFormat_e;

typedef struct
{
	XM_S32 nWidth;  /* ����YUV��ȡ����ʱ��ָ����ʼ�����Ϊ0����ȡ������ͨ����ӦVPSSͨ�������*/
	XM_S32 nHeight;	/* ����YUV��ȡ����ʱ��ָ����ʼ�ߡ����Ϊ0����ȡ������ͨ����ӦVPSSͨ������ߡ�*/
	XM_U8  aReserved[8]; /* ������*/	
} MaQueYuvCreate_s;

typedef struct
{
	XM_S32 nWidth;   /* ���ػ�ȡ��YUV����ʵ�ʿ����������õ�YUV���ͨ����*/
	XM_S32 nHeight;   /* ���ػ�ȡ��YUV����ʵ�ʸߡ����������õ�YUV���ͨ����*/
	XM_U8 *pBuffer;   /* ָ���ȡ��YUV����Bufferָ�롣*/
	XM_S32 nBufLen;   /* ��pBuffer������ڴ��С����λ���ֽڡ�*/
	XM_S32 nDataLen;  /* ʵ��YUV���ݴ�С��*/
	MaQueYuvPixelFormat_e ePixelFormat;  /* YUV���ݴ洢��ʽ��*/
	XM_U64 pts;    /* YUV����֡����ʾʱ�������λ:΢�롣*/
	XM_HANDLE handleMem;  /* �ɵײ�����ڴ�ʱ���ڴ���ǰ����ܵ�LibXmMaQue_Mem_init()����ע���pMemAllocFuncPtr
						   * ����õ��������Ҫ����һ��Memory Handle�������ߣ������ͷš����Memory Handle��
						   * �����ڱ���Ա�С�ע���ó�Ա������ֻ���ɵײ�����ڴ�ʱ��Ч�� */
	XM_U8  aReserved[8];  /* ������*/
} MaQueYuvFrame_s;

typedef XM_S32 (*MaQueYuvGetCallbackFuncPtr) (XM_VOID *pUserArg, MaQueYuvFrame_s *pstYuvFrame);

typedef struct 
{
	MaQueYuvGetCallbackFuncPtr pCallbackFuncPtr;  /* ��ȡYUV����֡�Ļص�����ָ�롣*/
	XM_VOID *pCallbackArg;	 /* �����˽�в��������ᴫ��pCallbackFuncPtr�ĵ�һ��������*/
} MaQueCallback_s;

typedef struct
{
	MaQueCallback_s stCallback;  /* ����ע���ȡYUV����֡�Ļص�����ָ�뼰ע����˽�в�����*/
	XM_U32 nWidth;    /* ������ץȡYUV�Ŀ�0����ȡ��ͨ����������ͨ����������ȡ��ͨ���Ŀ�*/
	XM_U32 nHeight;   /* ������ץȡYUV�ĸߡ�0����ȡ��ͨ���ߣ�������ͨ�������ߣ���ȡ��ͨ���ĸߡ�*/
	XM_U8 aReserved[8]; /* ������*/
} MaQueYuvParam_s;

typedef struct
{
	XM_S32 channel;  /* ץȡ��ͨ��������ȡֵ[0, 1]���ֱ��Ӧ��·YUV��ͨ��0��1��*/
	XM_U32 nWidth;   /* ��Ҫץȡ��YUV����ʵ�ʿ�*/
	XM_U32 nHeight;  /* ��Ҫץȡ��YUV����ʵ�ʸߡ�*/
	MaQueYuvPixelFormat_e ePixelFormat;  /* YUVͼ��֡�����ش洢��ʽ��*/
	XM_U64 pts;		 /* YUVͼ��֡����ʾʱ�������λ��΢�롣*/
	XM_U64 phyAddr;  /* ����YUVͼ��֡���ݵ������ַ��*/
	XM_U32 reserved;   /* ��������ʱû���á�*/
	XM_VOID *pYuvBuffer;  /* ����YUVͼ��֡���ݵ������ַ����Ӧ�ò���Է��ʡ�*/
	XM_U32 aStride[3];   /* ��ӦYUVͼ��֡���ݵ�Y��U��V��������ȣ���ÿ�����ڴ�����ռ�õ��ֽ�����*/
	XM_S32 index;     /* ֡���������������ʱ������֡���������ײ�ᴴ����Ӧ�ڴ�أ�����֡���档���
					   * �������ڱ�ʶ��ǰ֡ʹ�����ĸ�֡���棬���ں����ͷš�֡����������ο���
					   *	MaQue2YuvCreateParam_s��nCachedFrameCount��
					   */
	XM_HANDLE handleMem; /* �ײ��MMZ���������ڴ棬��������ڴ���Ϣ���������ṹ���С���Ҫ���ں�
						  * Jpeg���빲���ڴ棬����MaQueJpegFrame_s��handleMemһ�������Ի��ำֵ��
						  * ������������Jpeg����ӿڣ�ʵ��YUVתJpeg����������ֻ��ֲ���
						  * �ο���demo_2yuv2jpeg.c��
						  */
} MaQue2YuvFrame_s;

typedef struct
{
	XM_U32 nWidth;
	XM_U32 nHeight;
	XM_U32 nCachedFrameCount;
} MaQue2YuvCreateParam_s;

typedef XM_VOID (*MaQue2YuvGetCallbackFuncPtr)(MaQue2YuvFrame_s *pstYuvFrame1, MaQue2YuvFrame_s *pstYuvFrame2, XM_VOID *pUserData);

typedef struct 
{
	MaQue2YuvGetCallbackFuncPtr pCallbackFuncPtr;  /* ��ȡ2��YUV����֡�Ļص�����ָ�롣*/
	XM_VOID *pCallbackArg;	 /* �����˽�в��������ᴫ��pCallbackFuncPtr�ĵ�һ��������*/
} MaQue2YuvCallback_s;

typedef struct
{
	MaQue2YuvCreateParam_s astCreateParam[2];
	XM_U32 fps;
	MaQue2YuvCallback_s stCallback;  /* ����ע���ȡYUV����֡�Ļص�����ָ�뼰ע����˽�в�����*/
	XM_U8 aReserved[8]; /* ������*/
} MaQue2YuvCreate_s;


/*
������������
	������ȡYUV���ݵ�����
��������
	  ��������				����
	channel [in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstYuvCreate [in] 	���ڴ�����ȡYUV��������Ĳ�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_YUV_create(XM_S32 channel, MaQueYuvCreate_s *pstYuvCreate);

/*
������������
	�ı�YUV���ݿ�ߵȲ�����Ҳ����ͨ����APIע���ȡYUV���ݵĻص�������

��������
	  ��������				����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstYuvParam [in]	����YUV���ݻ�ȡ��������߻�ص���������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	ǰ��LibXmMaQue_YUV_create()�����������pstYuvCreateָ��YUV��ߡ�LibXmMaQue_YUV_setParam()
	���޸Ŀ�ߡ�������ܵ�LibXmMaQue_YUV_getFrame()�ڶ�������pstYuvFrame��Ҳ���漰����ߣ����
	�����޸�YUV���ͨ����ߣ���ֻ����YUV���ݵ�ʵ�ʿ�ߡ�
*/
XM_S32 LibXmMaQue_YUV_setParam(XM_S32 channel, MaQueYuvParam_s *pstYuvParam);

/*
������������
	��ѯ��ʽ����ȡһ֡YUV���ݡ�

��������
	  ��������				����
	channel [in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstYuvFrame	[out] 	����ȡYUV����֡��Ϣ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	pstYuvFrame��ָ���Ŀ�ߣ��������õ�YUV���ͨ������ֻ����YUV���ݵ�ʵ�ʿ�ߡ�Ҫ�޸�
	YUVͨ���Ŀ�ߣ�Ӧ����ǰ����ܵ�API��LibXmMaQue_YUV_setParam()��
*/

XM_S32 LibXmMaQue_YUV_getFrame(XM_S32 channel, MaQueYuvFrame_s *pstYuvFrame);


/*
������������
	������LibXmMaQue_YUV_create()�����Ļ�ȡYUV����
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_YUV_destroy(XM_S32 channel);

/*
������������
	������ȡ��·YUVͼ��֡����������·YUV��ͬԴ�ģ�Ҳ��Сͼ�ɴ�ͼ��С���á���·YUVͼ��֡��ͨ��ע����ȥ�Ļص������ص������ġ�
��ԭ�͡�
	XM_S32 LibXmMaQue_2YUV_create(XM_S32 channel, MaQue2YuvCreate_s *pst2YuvCreate);
��������
	  ��������					����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstYuvCreate [in]	���ڴ�����ȡ2·YUV��������Ĳ�����
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_2YUV_create(XM_S32 channel, MaQue2YuvCreate_s *pst2YuvCreate);

/*
������������
	�ͷ�����ȡ����·YUVͼ��֡������һ֡�������ͷ�֡����������pstYuvFrame�С�pstYuvFrame�ǵײ�ص����صĽṹ�壬ԭ�����뼴�ɡ�
��ԭ�͡�
	XM_S32 LibXmMaQue_2YUV_releaseFrame(XM_S32 channel, MaQue2YuvFrame_s *pstYuvFrame);
��������
	  ��������					����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstYuvFrame	[in]	��Ҫ�ͷŵ�YUVͼ��֡��Ϣ��
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_2YUV_releaseFrame(XM_S32 channel, MaQue2YuvFrame_s *pstYuvFrame);

/*
������������
	������LibXmMaQue_2YUV_create()����������
��ԭ�͡�
	XM_S32 LibXmMaQue_2YUV_destroy(XM_S32 channel);
��������
	  ��������					����
	channel	[in]		��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
��ע�⡿
	���LibXmMaQue_2YUV_create()�����������ȡYUVͼ��֡������Jpeg���룬����Ҫ��ֹͣJpeg��������
	���ɵ��ñ�API������API�ͷ���Jpeg���빲���VB�飬�ᵼ�³��������
*/
XM_S32 LibXmMaQue_2YUV_destroy(XM_S32 channel);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_YUV_H


