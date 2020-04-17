
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_mem.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示内存分配和释放函数注册。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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
 * Video和Jpeg类型的编码帧，往往数据量很大，而编码器中传出来的数据是分段的，底层需要一次内存拷贝，
 * 才能拼成一帧。直接底层库中申请内存，可能会申请失败，也会带来太多内存碎片，不够优化。特提供了一
 * 套申请或释放内存的接口，供应用开发者实现。本例中，为了演示简洁明了，用了常用的malloc，实际完全
 * 可以用事先分配的内存块(如环形缓存)优化，减少申请和释放消耗。
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

