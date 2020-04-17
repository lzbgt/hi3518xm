
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_mem.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����ע���ڴ���亯��ָ��Ľӿڡ�
 *		���ײ�ע���ڴ������ͷŽӿڡ���Ƶ��Jpeg�����������������С��һ�����Ƿֶεģ�Ҫƴ��
 *		һ֡�������ݣ������ǲ��ɱ���ģ�����������Ҫ�������ڴ档����뷨�ǣ��ײ�������
 *		�̸߳���ʵ��������С������malloc���������ڴ棬�ϲ㸺���ͷţ���������̫���뻯�ˣ���Ϊ
 *		�ڴ����ʱ���������ڴ潫��ʮ�����ѣ���ʱ������ʧ�ܣ�����������ͷ��ڴ棬�ؽ����
 *		��Ƭ�����أ���һ���񻯡�MaQueʵ��ʱ����ֿ�������Щ���أ��ṩ��һ�׽ӿڣ��ɶ��ο�����
 *		ʵ���Լ��ڴ������ͷ��Ż����򡣿����߿����ڸտ�ʼ��App�ڴ�ܳ�ԣ��ʱ�������Ƭ�ڴ�
 *		�����ڴ�أ�дһ���ȷ��㣬�ָ�Ч���ڴ�����������磬���λ������������������Ҫ�õ�����
 *		�ڴ�ģ�������������������ڴ�ӳ��ʵ�֡�
 *		Ŀǰ��Ҫ�õ������ڴ������ͷŽӿ�ģ����MaQueMemoryType_e������
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_MEM_H
#define MAQUE_MEM_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	MAQUE_MEM_TYPE_VIDEO_ENC = 1,   /* ��Ƶ���롣*/
	MAQUE_MEM_TYPE_JPEG_ENC,  /* Jpeg���롣*/
	MAQUE_MEM_TYPE_YUV_GET	  /* YUV��ȡ��*/
} MaQueMemoryType_e;

typedef struct
{
	MaQueMemoryType_e eMemType;  /* �ڴ���������͡�ָ����������ڴ������ĸ�ģ�顣*/
	XM_S32 subType;   /* ��Ӧ��eMemType�������͡�eMemTypeָֻ����һ�����࣬����Video Enc��ʵ����Ƶ
					   * ���������������͸�����֮�֣����øñ����������֡�Ŀǰ����ֻ������Ƶ���룬
					   * subType��Ӧ�� MaQueStreamChannel_e��
					   */
	XM_U32 nBufSize;  /* ��Ҫ�����Buffer��С��*/
	XM_U32 nAlign;    /* �����ڴ����ʼ��ַ�����ֽ�����*/
	XM_U8 *pBuffer;   /* ָ�����뵽���ڴ���ʼ��ַ��*/
	XM_U8 aReserved[8];  /* ������*/
} MaQueMemAllocParam_s;

typedef XM_S32 (*MaQueMemoryAllocFuncPtr)(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam);
typedef XM_S32 (*MaQueMemoryReleaseFuncPtr)(XM_HANDLE handle);
typedef XM_S32 (*MaQueMemoryAddRefFuncPtr)(XM_HANDLE handle);
typedef XM_S32 (*MaQueMemorySetLengthFuncPtr)(XM_HANDLE handle, XM_U32 len);

typedef struct 
{
	MaQueMemoryAllocFuncPtr  pMemAllocFuncPtr;   /* �����ڴ�ص���*/
	MaQueMemoryReleaseFuncPtr pMemReleaseFuncPtr;  /* �ͷ��ڴ�ص������ʹ�òο������ƣ����ȰѲο�����1���Ȳο���Ϊ0ʱ���ͷ��ڴ档*/
	MaQueMemoryAddRefFuncPtr pMemAddRefFuncPtr;  /* ���ڴ�ʹ�òο�����1��*/
	MaQueMemorySetLengthFuncPtr pMemSetLenFuncPtr;  /* ����������ڴ�ʵ��ʹ������*/
} MaQueMemoryApi_s;


/*
������������
	���ײ�ע���ڴ������ͷŽӿڡ�ĿǰӦ������Ƶ��Jpeg����������Լ�YUV���ݻ�ȡ��
��������
	  ��������			����
	pstMemApi [in]	ע����ײ���ڴ������ͷŽӿڡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	Ŀǰ��Ҫ�õ������ڴ������ͷŽӿڵ�ģ����MaQueMemoryType_e������Ҳ���ϲ����
	ʵ��MaQueMemoryType_e��������������ڴ���䡣
*/
XM_S32 LibXmMaQue_Mem_init(MaQueMemoryApi_s *pstMemApi); 

#ifdef __cplusplus
}
#endif


#endif //MAQUE_MEM_H


