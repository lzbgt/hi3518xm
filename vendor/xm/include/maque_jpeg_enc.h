
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_jpeg_enc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����Jpeg������ؽṹ�壬����Jpeg������ؽӿڣ����ⲿʹ�á�
 *		�ṩ�˴���Jpeg���룬��ȡJpeg����֡�Ƚӿڡ�Jpeg�������������������Sensor���VI���룬
 *    Ҳ�����ϲ������ֱ�Ӵ���YUV����֡������������"��Sensor���VI����ͼ��֡"��"���ϲ㴫���
 *    YUVͼ��֡"�����֡����߹���һ�׽ṹ�壬��˴��η�ʽ���ƣ��������Ա��������ȴ�����Բ��
 *    ����ο����������"���ϲ㴫���YUVͼ��֡"API����"LibXmMaQue_JpegEnc_Yuv_"��Ϊǰ׺��
 *    ��Sensor���VI����ͼ��֡��
 *           ���������Jpegͼ��֡����ͨ����ѯ��ע��ص���ʽ��ȡ��ע�⣬�ص��Ͳ�ѯ��ʽ����ͬʱ
 *       ʹ�á���ѯ��ʽ��ÿ�ζ������޸�ͼ���ߣ����ص�����ֻ�ڵ���ע�ắ��ʱ����һ�Σ���ˣ�
 *       ���ص��Ͳ�ѯͬʱʹ��ʱ���ص����ص�ͼ���СΪ���һ�����õ�����ͨ���Ĵ�С��Ҳ���ǣ���
 *       ע��ص����ٵ��ò�ѯ��ʽ�����޸��˿�ߣ���ô�ص����صĿ��Ҳ��֮�ı䡣
 *           Jpeg����֡��ȡ���������ᴫ�����ȥ���޸ı���ͨ����С����������ܵ�YUV���ݻ�ȡ����
 *       ����maque_yuv.h�½ڣ���ֻ��LibXmMaQue_YUV_create()��LibXmMaQue_YUV_setParam()���޸�ͨ
 *		 ����С������ȡ���������޸�ͨ����С��
 *	  ���ϲ㴫���YUVͼ��֡��
 *		 �ɵ����ߴ���YUV����֡������Ϣ��������ɺ�ͨ��ͬһ���ṹ�巵��Jpeg���ݼ�����Ϣ��
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_JPEG_ENCODE_H
#define MAQUE_JPEG_ENCODE_H


#include "maque_struct.h"
#include "maque_yuv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_S32 nWidth; /* ����ͼ�����ʵ�ʿ���Sensor���VI����ͼ��֡��nWidth��nHeight��һΪ0������ȡ�ͻ����Ƶ����ץͼ��ߡ�
	                * ���ϲ㴫���YUVͼ��֡��
					*	nWidth��nHeight���벻С��YUV֡��ʵ��ߣ�������Ϊ0������YUV֡��ʵ���������ģ���������YUV֡��ʵ
					*   ��߻�Ҫ��ı�����Ҳ�ǿ��Եģ�ֻ������������Դ��
	                */
	XM_S32 nHeight;  /* ����ͼ�����ʵ�ʸߡ���Sensor���VI����ͼ��֡��nWidth��nHeight��һΪ0������ȡ�ͻ����Ƶ����ץͼ��ߡ�
	                  *  ���ϲ㴫���YUVͼ��֡��
					  *	  nWidth��nHeight���벻С��YUV֡��ʵ��ߣ�������Ϊ0������YUV֡��ʵ���������ģ���������YUV֡��ʵ
					  *   ��߻�Ҫ��ı�����Ҳ�ǿ��Եģ�ֻ������������Դ��
	                  */
	XM_U8 aReserved[8];  /* ������*/
} MaQueJpegEncCreate_s;

typedef struct
{
	XM_U32 nWidth;    /* ��Sensor���VI����ͼ��֡��
	                   *        ���ڲ�ѯ��ʽץͼʱ��������ץȡͼƬ�Ŀ�����Ϊ0�򳬹�������ȡ����������ʱ֧�ֵ�����ȡ�
					   *    ��ѯ��ص���ʽ�����ոñ����ᱣ��ʵ��Jpegͼ����ء����ײ���ܻ��޸ĸñ���ֵ��
					   * ���ϲ㴫���YUVͼ��֡��
					   *    ����ʱ������YUV֡����ʵ�����ر���ʵ�ʿ����YUV����2����ģ�����ͷ��ؿ�һ�¡�
					   */
	XM_U32 nHeight;   /* ��Sensor���VI����ͼ��֡��
	                   *	    ���ڲ�ѯ��ʽץͼʱ��������ץȡͼƬ�ĸߣ�����Ϊ0�򳬹�������ȡ����������ʱ֧�ֵ����߶ȡ�
					   *    ��ѯ��ص���ʽ�����ոñ����ᱣ��ʵ��Jpegͼ��߷��ء����ײ���ܻ��޸ĸñ���ֵ��
					   * ���ϲ㴫���YUVͼ��֡��
					   *	����ʱ������YUV֡����ʵ�ߡ����ر���ʵ�ʸߣ����YUV����2����ģ�����ͷ��ظ�һ�¡�
					   */
	XM_U8 *pBuffer;   /* ��Sensor���VI����ͼ��֡������Jpeg����ͼ��֡���ݡ������ɵ����߷��䣬Ҳ�����ɵײ���䡣
					   * ���ϲ㴫���YUVͼ��֡�� ������YUV���ݣ������Ǳ�����Jpeg���ݡ�������LibXmMaQue_MMZ_malloc()����
					   * ���õ������ַ��
					   */
	XM_S32 nBufLen;   /* ��Sensor���VI����ͼ��֡��
	                   *       ��pBuffer������ڴ��С����λ���ֽڡ����nBufLen > 0��ͬʱ pBuffer��NULL��˵���ɵ����߸�
					   *    pBuffer�����ڴ棻�����ɵײ�����ڴ棬�����߸����ͷš�
					   *       ע���ɵײ�����ڴ�ʱ���ײ��ǵ����� LibXmMaQue_Mem_init()ע����ȥ���ڴ����ӿ�ʵ�ֵģ����
					   *    ��Ҫ���׻����ͷš�����ο���maque_mem.h�½ڡ�
					   *  ���ϲ㴫���YUVͼ��֡��������YUV���ݳ��ȣ��ײ㲻���޸ġ�
					   */
	XM_S32 nDataLen;  /* ��Sensor���VI����ͼ��֡������ʵ��Jpeg������С��
					   * ���ϲ㴫���YUVͼ��֡��������YUV���ݳ��ȣ������Ǳ����Jpeg���ݳ��ȡ�
					   */
	XM_U64 pts;	      /* ��Sensor���VI����ͼ��֡��Jpeg����ʱϵͳʱ�䣬��λ��΢�롣
					   * ���ϲ㴫���YUVͼ��֡��
					   *	 ���Դ���ʱ�����Ҳ���Դ���0���ɵײ㸳����ʱ��ʱ�������λ��Ϊ΢�룩��
					   */
	XM_HANDLE handleMem;  /* ��Sensor���VI����ͼ��֡��
	                       *       �ɵײ�����ڴ�ʱ���ڴ������Ľ��ܵ�LibXmMaQue_Mem_init()����ע���pMemAllocFuncPtr����
						   *   �õ��������Ҫ����һ��Memory Handle�������ߣ������ͷš����Memory Handle�ͱ����ڱ���Ա
						   *   �С�ע���ó�Ա������ֻ���ɵײ�����ڴ�ʱ��Ч��
						   * ���ϲ㴫���YUVͼ��֡��LibXmMaQue_MMZ_malloc()���ص�pstMmzMalloc->handleMem����ֻ��LibXmMaQue
						   *	���ڲ�ʹ�ã�������ֻ�谴ԭֵ���뼴�ɡ�
						   */
	MaQueYuvPixelFormat_e eYuvPixFmt;  /* ��Sensor���VI����ͼ��֡��û���á�
										* ���ϲ㴫���YUVͼ��֡��YUV����֡�����ظ�ʽ��
										*/
	MaQueRect_s stCropRect;  /* ����Jpeg�ü�����ֻ����ֲ�ͼ��ֻҪwidth��height��һΪ0���������á� */
	XM_U8 aReserved[8]; /* ������*/
} MaQueJpegFrame_s;

typedef XM_S32 (*MaQueJpegGetCallbackFuncPtr) (XM_VOID *pUserArg, MaQueJpegFrame_s *pstJpegFrame);

typedef struct 
{
	MaQueJpegGetCallbackFuncPtr pCallbackFuncPtr;  /* ��ȡJpeg�������ݵĻص�������*/
	XM_VOID *pCallbackArg;	 /* ע��ص�����ʱ�������ע����˽�нṹ�塣�����ڵ��ûص�����ʱ��������һ��������
							  * ֻ��ע����ʹ�ã��ײ㲻���ľ������ݣ���˿�ΪNULL��
							  */
} MaQueJpegCallback_s;

typedef struct
{
	MaQueJpegCallback_s stCallback;  /* ͨ���ص���ʽ��ȡJpeg�������֡����ע��Ļص�����ָ���ע����˽�нṹ�塣*/
	XM_U32 nWidth;    /* ����ͼ�����ʵ�ʿ�nWidth��nHeight��һΪ0������ȡ�ͻ����Ƶ����ץͼ��ߡ�*/
	XM_U32 nHeight;   /* ����ͼ�����ʵ�ʸߡ� nWidth��nHeight��һΪ0������ȡ�ͻ����Ƶ����ץͼ��ߡ�*/
	XM_U8 aReserved[8]; /* ������*/
} MaQueJpegCallbackParam_s;

/*
������������
	����JpegӲ���������ο����桾ע�⡿˵������APIֻ�Ƿ��سɹ���
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstCreate [in]	����Jpeg������������Ŀǰ��ʱ����Ҫ���ã��ɴ�NULL��
������ֵ��
	XM_SUCCESS	�ɹ���
	����	ʧ�ܣ���ο������롣
��ע�⡿
	Jpeg��������Ӳ��ģ��ʵ�ֵġ����ײ������Ԥ������ڴ��С���ޣ���ֹ�������ٻ򴴽���
	����ڴ���Ƭ����������޷��ٴ����ɹ�����ˣ�Jpeg��������LibXmMaQue_System_startUp()ʱ��
	�Ͱ��ͻ���������ֱ��ʣ���sensor�йأ�������ֻҪ����ͼ���С�仯�����ڴ���ʱ���
	�ֱ��ʣ��Ϳ��Բ������ٱ�������
*/
XM_S32 LibXmMaQue_JpegEnc_create(XM_S32 channel, MaQueJpegEncCreate_s *pstCreate);

/*
������������
	ע��ص���������ȡJpeg����ͼ��֡��
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstCallbackParam [in]	ע����ײ��ȡJpeg����ͼ��֡�Ļص�������ͬʱ����ͼ��Ŀ�ߡ����
							����һΪ0��������������ʱ�趨�����ֱ��ʱ��롣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	ע��ص���ʽ��ȡJpeg����֡��Ҫ��Jpeg����һֱ�����У�ϵͳ�����ܴ�ͬʱҪ������ߣ���ʱ
	����ÿһ֡ͼ�����¶���������������ᵼ�µײ��ڴ��ò����ͷţ����ղ���Ӱ��Jpeg������
	Ӱ����Ƶ���루��Jpeg����Ƶ�����в����ڴ���ǹ���ʹ�õģ���ͨ���ص���ʽ��ȡJpeg����֡��
	���Ժܼ�ʱ�ط���ÿ֡ͼ������������ץͼ�������������ץͼ�������ý�������Ҫ���ܵĲ�ѯ
	��ʽ��ȡ����ο�API��LibXmMaQue_JpegEnc_getFrame()��
	ֻ����ñ�APIһ�Σ�ע����ص������󣬵ײ�ÿ���һ֡ͼ�񣬶��ᴫ���ص�������Ҫ��ֹͣ�ص�
	��ʽ��ȡJpeg����֡��ֻ��Ҫ�ٵ���һ�α�API����ע��Ļص�����ָ���ó�NULL��
*/
XM_S32 LibXmMaQue_JpegEnc_registerCallback(XM_S32 channel, MaQueJpegCallbackParam_s *pstCallbackParam);

/*
������������
	ͨ����ѯ��ʽ����ȡJpeg����ͼ��֡��
��������
	  ��������					����
	channel	[in]			��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstJpegFrame [int/out]	��ȡJpeg����ͼ��֡�Ĳ�������������ͼ��Ŀ�ߣ������ɵ����ߴ������
							�õ�Buffer��Ҳ�����ɵײ㰴ʵ�ʱ���֡�Ĵ�С����Buffer���ɵ����߸���
							�ͷ�Buffer������ο���MaQueJpegFrame_s��Ա˵����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	ͨ����ѯ��ʽ��ȡJpeg����֡��������Jpeg��֡����ģʽ����ÿ������һ֡����ͣ������ϵͳ����
	��С��Ҫ����ץͼ����ο�ǰ����ܵ�API��LibXmMaQue_JpegEnc_registerCallback()��
*/
XM_S32 LibXmMaQue_JpegEnc_getFrame(XM_S32 channel, MaQueJpegFrame_s *pstJpegFrame);

/*
������������
	����Jpeg��������Ŀǰ����ʱ��֧��Jpeg�������٣�ֻ�ǰ�ǰ��ע��Ļص���ա�
��������
	  ��������			����
	channel	[in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����		ʧ�ܣ���ο������롣
��ע�⡿
	Ŀǰ��APIʵ�ֵĹ��ܣ��൱�ڵ���LibXmMaQue_JpegEnc_registerCallback()�������лص�����
	ָ������ΪNULL��
*/
XM_S32 LibXmMaQue_JpegEnc_destroy(XM_S32 channel);

/*
������������
	����YUVתJpegӲ��������
��������
	  ��������			����	
	channel [in]	��Ӧ����ͨ������ֻ֧��һ��ͨ����Ӧ����Ϊ0��	
	pstCreate [in]	����Jpeg�������������ṹ����nWidth��nHeight���벻С��YUV֡��ʵ��ߣ�������Ϊ0��	
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��ǰ��LibXmMaQue_JpegEnc_create()��ȣ�������ʽ��ͬ��������ṹ�����pstCreate��nWidth��nHeight
	���벻С��YUV֡��ʵ��ߡ�����YUV֡��ʵ���������ģ���������YUV֡��ʵ��߻�Ҫ��ı�����Ҳ�ǿ���
	�ģ�ֻ������������Դ��
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_create(XM_S32 channel, MaQueJpegEncCreate_s *pstCreate);

/*
������������
	��һ֡YUV���ݱ����JpegͼƬ��
��������
	  ��������				����
	channel [in]		��Ӧ����ͨ������ֻ֧��һ��ͨ����Ӧ����Ϊ0��
	pstFrame [in/out]	����ʱ������YUV����֡������Ϣ������ʱ������Jpeg����������Ϣ������ʱ���ṹ����
						��nWidth��nHeight�����趨ΪYUV��ʵ��ߣ����ر���ʵ�ʿ�ߣ����YUV�����2���룬
						����ͷ��ؿ��һ�¡�pBuffer������YUV���ݣ������Ǳ�����Jpeg���ݡ�nBufLen��
						����YUV���ݳ��ȣ��ײ㲻���޸ġ�nDataLen������YUV���ݳ��ȣ������Ǳ����Jpeg����
						���ȡ�pts���Դ���ʱ�����Ҳ���Դ���0���ɵײ㸳����ʱ��ʱ�������λ��Ϊ΢�룩��
						handleMem��LibXmMaQue_MMZ_malloc()���ص�pstMmzMalloc-> handleMem��eYuvPixFmt��
						YUV����֡�����ظ�ʽ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	��ǰ��LibXmMaQue_JpegEnc_getFrame()��ȣ�������ʽһ�������ṹ�����pstFrame�ĳ�Ա����������ܴ�
	�ο�"��������"�����ϸ˵����YUV��ߵײ�ᰴ2���봦�����������ż����YUV��߲ο�Jpeg���������ƣ�
	��СΪ32��
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_encode(XM_S32 channel, MaQueJpegFrame_s *pstFrame);

/*
������������
	����YUVתJpeg��������
��������
	  ��������			����
	channel [in]	��Ӧ����ͨ������ֻ֧��һ��ͨ����Ӧ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_JpegEnc_Yuv_destroy(XM_S32 channel);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_JPEG_ENCODE_H


