
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_mmz.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.04.04
 * ��������: ����MMZ���ڴ�������ͷ���صĽӿڡ���Щ���ϣ����磬��Ƶ��ͼƬ���룩��Ҫ�õ�
 * 		����ڴ棬������Ҫ�õ����������һ�µ��ڴ棬���Ԥ����MMZ��Memory Management Zone����
 *		��������Ч��ֹ�˱����ϵͳ�ڴ��Ӱ�졣��Щ�ڴ�����Linux����ϵͳ���õ�OS Mem�޹�������
 *		����ͨ���ڴ�ӳ�䷽ʽʹ�á�Ŀǰ��Ҫ�õ������ڴ������ͷŽӿ�ģ���У�YUVתJpeg���롣
 *
 * �޸���ʷ:  
 ** 1.����: 2019.04.04
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.04.04
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_MMZ_H
#define MAQUE_MMZ_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_U32 nMallocLen;		/* ��������ڴ���С���ɵ����ߴ��롣*/
	XM_VOID *pPhyAddr;  	/* ��MMZ(Memory Management Zone)����������ڴ���ʼ��ַ���ײ㷵�ء�*/
	XM_VOID *pVirtAddr;		/* pPhyAddrͨ���ڴ�ӳ�����õ������ڴ���ʼ��ַ���ײ㷵�ء�*/
	XM_HANDLE handleMem;	/* �ڴ����handle�������ڲ�ʹ�ã��ⲿ������ֻ��Ҫ�Ѹ�handle��ԭֵ���롣�ײ㷵�ء�*/
	XM_U8 aReserved[8]; 	/* ������*/
} MaQueMmzMalloc_s;


/*
������������
	��Ԥ����MMZ��Memory Management Zone�������ڴ�顣MMZ��Linux����ϵͳ���õ�OS Mem�޹�����������
	ͨ���ڴ�ӳ�䷽ʽʹ�á���API��MMZ������һ���ڴ棬���ӳ��󣬷�������������ڴ��ַ��
��������
	  ��������					����
	pstMmzMalloc [in/out]	��MMZ�������ڴ�Ĳ����������߸���Ա����nMallocLen��ֵ��������Ա�����ɵײ㸳ֵ���ء�
������ֵ��
	XM_SUCCESS			�ɹ���
	����				ʧ�ܣ���ο������롣
��ע�⡿
	YUVתJpeg�����У�YUV����ͨ��LibXmMaQue_JpegEnc_Yuv_encode()�������ײ�����������YUV���ݵ�Buffer����
	ͨ����API�������ã����Ҫע��pstMmzMalloc->nMallocLen�Ĵ�С���㡣����YUV֡��ʵ��ߺ����ظ�ʽ�йء�
	�����߷ֱ�Ϊ��nWidth��nHeight�����ظ�ʽΪMAQUE_YUV_PIXEL_FORMAT_YVU_SP420����ôpstMmzMalloc->nMallocLen
	Ϊ����nWidth * nHeight��* 3 / 2��ע�⣺ʵ�ʴ��ڸ�ֵҲ������ģ�ֻ�������˷��ڴ档
*/
XM_S32 LibXmMaQue_MMZ_malloc(MaQueMmzMalloc_s *pstMmzMalloc);

/*
������������
	�ͷ���ǰ��LibXmMaQue_MMZ_malloc()������ڴ�顣���ߵĲ�����ͬ��
��������
	  ��������				����
	pstMmzMalloc [in]	ǰ����ܵ�API��LibXmMaQue_MMZ_malloc()�ķ���ͬ���ṹ�����
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_MMZ_free(MaQueMmzMalloc_s *pstMmzMalloc);


#ifdef __cplusplus
}
#endif

#endif
