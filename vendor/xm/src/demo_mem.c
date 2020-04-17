
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_mem.c
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ�ڴ������ͷź���ע�ᡣ
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#include "demo_inc.h"
#include "demo_mem.h"


#define UVC_MEM_SIZE 1843200
#define UVC_MEM_BANKS 1
#define UAC_MEM_SIZE 4096
#define UAC_MEM_BANKS 4

typedef enum 
{
    MEMORY_TYPE_UVC,
    MEMORY_TYPE_UAC, 
    MEMORY_TYPE_NORMAL,
} MemoryType_e;


typedef struct {
	XM_U8 *pBuffer;	
	XM_U32 nBufSize;
	XM_U32 nUsedBytes;
	XM_U32 nRefCount;
	XM_S32 index;
} DemoMemory_s;


static pthread_mutex_t g_mutexMem = PTHREAD_MUTEX_INITIALIZER;


/*
 * Video��Jpeg���͵ı���֡�������������ܴ󣬶��������д������������Ƿֶεģ��ײ���Ҫһ���ڴ濽����
 * ����ƴ��һ֡��ֱ�ӵײ���������ڴ棬���ܻ�����ʧ�ܣ�Ҳ�����̫���ڴ���Ƭ�������Ż������ṩ��һ
 * ��������ͷ��ڴ�Ľӿڣ���Ӧ�ÿ�����ʵ�֡������У�Ϊ����ʾ������ˣ����˳��õ�malloc��ʵ����ȫ
 * ���������ȷ�����ڴ��(�绷�λ���)�Ż�������������ͷ����ġ�
 */
XM_S32 MaQue_Demo_Mem_alloc(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam)
{
	DemoMemory_s *pstMem = NULL;
	
	pstMem = (DemoMemory_s *)malloc(sizeof(DemoMemory_s));
    if (!pstMem) 
	{
        ERROR_PRINTF("malloc failed!\n");
        return -1;
    }
    memset(pstMem, 0, sizeof(*pstMem));
	
    pthread_mutex_lock(&g_mutexMem);

	switch(pstAllocParam->eMemType)
	{
		case MAQUE_MEM_TYPE_VIDEO_ENC:
		case MAQUE_MEM_TYPE_JPEG_ENC:
		case MAQUE_MEM_TYPE_YUV_GET:
			pstMem->pBuffer = (XM_U8 *)malloc(pstAllocParam->nBufSize);	
		    if (pstMem->pBuffer) 
			{
		        pstMem->index = 0xff;
				pstMem->nBufSize = pstAllocParam->nBufSize;
		        pstMem->nRefCount = 1;
		    } 
			else 
			{
		        free(pstMem);	
		        pstMem = NULL;
		    }
			break;

		default:
			ERROR_PRINTF("Invalid eMemType: %d!\n", pstAllocParam->eMemType);
			break;
	}
	
    pthread_mutex_unlock(&g_mutexMem);

    if (!pstMem) 
	{
        ERROR_PRINTF("alloc failed\n");
        return -1; 
    }

    *pHandle = (XM_HANDLE)pstMem;
    pstAllocParam->pBuffer = pstMem->pBuffer;

	return 0;	
}

XM_S32 MaQue_Demo_Mem_release(XM_HANDLE handle)
{
	DemoMemory_s *pstMem = (DemoMemory_s *)handle;
	
	if (!handle) 
	{
		return -1;
	}
	
    pthread_mutex_lock(&g_mutexMem);
    if (pstMem->nRefCount > 1) 
	{
       pstMem->nRefCount--; 
    } 
	else if (pstMem->pBuffer) 
	{
        free(pstMem->pBuffer);
        free(pstMem);
    }
	
    pthread_mutex_unlock(&g_mutexMem);

	return 0;	
}

XM_S32 MaQue_Demo_Mem_setLength(XM_HANDLE handle, XM_U32 len)
{
	DemoMemory_s *pstMem = (DemoMemory_s *)handle;

	pstMem->nUsedBytes = len;

	return 0;
}

XM_S32 MaQue_Demo_Mem_addRef(XM_HANDLE handle)
{
	DemoMemory_s *pstMem = (DemoMemory_s *)handle;

    pthread_mutex_lock(&g_mutexMem);
	
    pstMem->nRefCount += 1;
	
    pthread_mutex_unlock(&g_mutexMem);

	return 0;
}

