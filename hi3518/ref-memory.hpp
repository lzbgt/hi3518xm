#ifndef __REF_MEMORY_H__
#define __REF_MEMORY_H__

#include <maque_type.h>
#include <maque_mem.h>
#include <pthread.h>
#include <spdlog/spdlog.h>
#include <evpacket.h>

#ifdef __cplusplus
extern "C" {
#endif

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

XM_S32 MaQue_Demo_Mem_setLength(XM_HANDLE handle, XM_U32 len)
{
    DemoMemory_s *pstMem = (DemoMemory_s *)handle;
    pstMem->nUsedBytes = len;

    return 0;
}

volatile uint32_t cntAlloc = 0, cntFree = 0;

XM_S32 MaQue_Demo_Mem_alloc(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam)
{
    DemoMemory_s *pstMem = NULL;

    pthread_mutex_lock(&g_mutexMem);

    pstMem = (DemoMemory_s *)malloc(sizeof(DemoMemory_s));
    memset(pstMem, 0, sizeof(*pstMem));
    if (!pstMem) {
        spdlog::error("malloc failed!");
        return -1;
    }

    switch(pstAllocParam->eMemType) {
    case MAQUE_MEM_TYPE_VIDEO_ENC:
    case MAQUE_MEM_TYPE_JPEG_ENC:
    case MAQUE_MEM_TYPE_YUV_GET:
        /// NOTE by Bruce: ATTENTION! we place the extr evpacket_t ahead for saving memory ops later
        XM_U8 * raw = (XM_U8 *)malloc(pstAllocParam->nBufSize + sizeof(evpacket_t));
        pstMem->pBuffer =  raw + sizeof(evpacket_t);
        spdlog::debug("malloc. raw: {0:x}, shifted: {0:x}", (uint32_t)raw, (uint32_t)pstMem->pBuffer);

        if (pstMem->pBuffer - sizeof(evpacket_t)) {
            pstMem->index = 0xff;
            pstMem->nBufSize = pstAllocParam->nBufSize;
            pstMem->nRefCount = 1;
        }
        else {
            free(pstMem);
            pstMem = NULL;
        }
        break;

    default:
        spdlog::error("Invalid eMemType: {}", pstAllocParam->eMemType);
        break;
    }

    if (!pstMem) {
        spdlog::error("alloc failed");
        return -1;
    }

    *pHandle = (XM_HANDLE)pstMem;
    pstAllocParam->pBuffer = pstMem->pBuffer;
    cntAlloc++;
    pthread_mutex_unlock(&g_mutexMem);

    return 0;
}

XM_S32 MaQue_Demo_Mem_release(XM_HANDLE handle)
{
    DemoMemory_s *pstMem = (DemoMemory_s *)handle;
    
    if (!handle) {
        return -1;
    }
    pthread_mutex_lock(&g_mutexMem);
    if (pstMem->nRefCount > 1) {
        pstMem->nRefCount--;
    }
    else if (pstMem->pBuffer && pstMem->pBuffer - sizeof(evpacket_t)) {
        /// NOTE by Bruce: ATTENTION!
        XM_U8* shifted = pstMem->pBuffer;
        XM_U8* raw = shifted - sizeof(evpacket_t);
        cntFree++;
        spdlog::debug("release. raw: {0:8x}, shifted: {0:8x}, a: {0:d}, f:{0:d}", (int)raw, (int)shifted, cntAlloc, cntFree);
        free(pstMem->pBuffer - sizeof(evpacket_t));
        free(pstMem);
    }else{
        spdlog::error("shouldn't be here. a:{0:d}, f:{0:d}", cntAlloc, cntFree);
    }

    pthread_mutex_unlock(&g_mutexMem);

    return 0;
}

XM_S32 MaQue_Demo_Mem_addRef(XM_HANDLE handle)
{
    DemoMemory_s *pstMem = (DemoMemory_s *)handle;

    pthread_mutex_lock(&g_mutexMem);
    // memory already released
    if(!handle) {
        return -1;
    }

    pstMem->nRefCount += 1;

    pthread_mutex_unlock(&g_mutexMem);

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif