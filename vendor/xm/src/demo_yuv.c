
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_yuv.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示YUV数据获取相关功能。
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

#define DEMO_YUV_GET_DIR	"YuvGet"

#define DEMO_YUV_CALLBACK_FILE		"CallbackYuv#%d_%dx%d_%s.yuv"
#define DEMO_YUV_LIB_MALLOC_FILE	"LibMallocYuv#%d_%dx%d_%s.yuv"
#define DEMO_YUV_USER_MALLOC_FILE	"UserMallocYuv#%d_%dx%d_%s.yuv"

typedef enum 
{
	YUV_BUFFER_TYPE_USER_MALLOC,
	YUV_BUFFER_TYPE_LIB_MALLOC,
	YUV_BUFFER_TYPE_CALLBACK,
} YuvBufferType_e;

typedef struct
{
	XM_S32 *pFrameCount;
	XM_S32 bWriteFile;
} YuvGetCallbackArg_s;

typedef struct
{
	XM_S32 bEnabled;
	XM_S32 bWriteFile;
	XM_U32 nFrames;
	XM_S32 nCreateWidth;
	XM_S32 nCreateHeight;
	XM_S32 nCfgWidth;
	XM_S32 nCfgHeight;
} YuvGetModParam_s;

static XM_S32 g_bYuvStop;

static XM_VOID Demo_Yuv_printFrame(XM_CHAR *pOwner, MaQueYuvFrame_s *pstYuvFrame)
{
	SIMPLE_PRINTF("@%s->Demo_Yuv_printFrame():\n", pOwner);
	SIMPLE_PRINTF("\tnWidth: %d\n", pstYuvFrame->nWidth);
	SIMPLE_PRINTF("\tnHeight: %d\n", pstYuvFrame->nHeight);
	SIMPLE_PRINTF("\tpBuffer: 0x%lx\n", (XM_UL)pstYuvFrame->pBuffer);
	SIMPLE_PRINTF("\tnBufLen: %d\n", pstYuvFrame->nBufLen);
	SIMPLE_PRINTF("\tnDataLen: %d\n", pstYuvFrame->nDataLen);
	SIMPLE_PRINTF("\tePixelFormat: %d\n", pstYuvFrame->ePixelFormat);
	SIMPLE_PRINTF("\tepts: %lld\n", pstYuvFrame->pts);
}

static XM_U32 Demo_Yuv_getBufLen(XM_U32 width, XM_U32 height)
{
	return (((width + YUV_WIDTH_ALIGN_MASK) & ~YUV_WIDTH_ALIGN_MASK) * ((height 
		+  YUV_HEIGHT_ALIGN_MASK) & ~YUV_HEIGHT_ALIGN_MASK) * 3) >> 1;
}

static XM_S32 Demo_Yuv_makeFilePath(XM_CHAR *pFilePath, 
	YuvBufferType_e eBufType, XM_S32 frameSeq, XM_S32 nWidth, XM_S32 nHeight, MaQueYuvPixelFormat_e ePixFmt)
{
	XM_S32 res;
	XM_CHAR aTmpFile[256];

	res = mkdir (DEMO_YUV_GET_DIR, 0755);
	if (res != 0 && EEXIST != errno)
	{
		ERROR_PRINTF("mkdir Failed: %s, Error: %s!\n", DEMO_YUV_GET_DIR, strerror(errno));
		return XM_FAILURE;
	}
	else
	{
		INFO_PRINTF("mkdir OK: %s!\n", DEMO_YUV_GET_DIR);
	}

	strcpy(pFilePath, DEMO_YUV_GET_DIR);
	strcat(pFilePath, "/");

	switch(eBufType)
	{
		case YUV_BUFFER_TYPE_USER_MALLOC:
			
			sprintf(aTmpFile, DEMO_YUV_USER_MALLOC_FILE, frameSeq, nWidth, nHeight, 
				MAQUE_YUV_PIXEL_FORMAT_YVU_SP420 == ePixFmt ? "YVU_SP420" : "YUV_SP420");
			
			break;

		case YUV_BUFFER_TYPE_LIB_MALLOC:
			
			sprintf(aTmpFile, DEMO_YUV_LIB_MALLOC_FILE, frameSeq, nWidth, nHeight,
				MAQUE_YUV_PIXEL_FORMAT_YVU_SP420 == ePixFmt ? "YVU_SP420" : "YUV_SP420");
			
			break;

		case YUV_BUFFER_TYPE_CALLBACK:
			
			sprintf(aTmpFile, DEMO_YUV_CALLBACK_FILE, frameSeq, nWidth, nHeight,
				MAQUE_YUV_PIXEL_FORMAT_YVU_SP420 == ePixFmt ? "YVU_SP420" : "YUV_SP420");
			
			break;

		default:
			
			ERROR_PRINTF("Invalid eBufType: %d!\n", eBufType);
			
			break;
	}

	strcat(pFilePath, aTmpFile);

	INFO_PRINTF("pFilePath: %s!\n", pFilePath);

	return XM_SUCCESS;
}

/*
 * 回调函数中不能调用LibXmMaQue_YUV_setParam()，会死锁的。
 */
static XM_S32 Demo_Yuv_getFrame_callback (XM_VOID *pUserArg, MaQueYuvFrame_s *pstYuvFrame)
{
	XM_S32 res;
	YuvGetCallbackArg_s *pstCallbackArg = (YuvGetCallbackArg_s *)pUserArg;
	
	if (pstCallbackArg->bWriteFile)
	{
		XM_CHAR aFileName[256];
		
		res = Demo_Yuv_makeFilePath(aFileName, YUV_BUFFER_TYPE_CALLBACK, 
				*pstCallbackArg->pFrameCount, pstYuvFrame->nWidth, pstYuvFrame->nHeight, pstYuvFrame->ePixelFormat);
		if (res != XM_SUCCESS)
		{
			ERROR_PRINTF("Demo_Yuv_makeFilePath Failed!\n");
		}
		else
		{
			MaQue_Demo_File_writeSimply(aFileName, pstYuvFrame->pBuffer, pstYuvFrame->nDataLen);
		}
	}
	else
	{
		Demo_Yuv_printFrame("Demo_getYuv_callback()", pstYuvFrame);
	}

	(*pstCallbackArg->pFrameCount) += 1;

	// MaQue_Demo_Mem_release(pstYuvFrame->handleMem);  /* YUV回调方式获取，内存来自底层VB块，不需要释放。*/

	return XM_SUCCESS;
}

static XM_VOID *Demo_Yuv_getFrameByCallback_task(XM_VOID *pArg)
{
	XM_S32 res;
	MaQueYuvCreate_s stYuvCreate;
	MaQueYuvParam_s stYuvParam;
	XM_S32 nFrames = 0;
	YuvGetCallbackArg_s stCallbackArg;
	YuvGetModParam_s stYuvGetModParam = { 0 };
	StringValueGet_s astYuvGetValGet[] = 
	{
		{"bEnabled:", &stYuvGetModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.bEnabled)},
		{"bWriteFile:", &stYuvGetModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.bWriteFile)},
		{"nFrames:", &stYuvGetModParam.nFrames, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nFrames)},	
		{"nCreateWidth:", &stYuvGetModParam.nCreateWidth, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCreateWidth)},	
		{"nCreateHeight:", &stYuvGetModParam.nCreateHeight, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCreateHeight)},
		{"nCfgWidth:", &stYuvGetModParam.nCfgWidth, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCfgWidth)},	
		{"nCfgHeight:", &stYuvGetModParam.nCfgHeight, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCfgHeight)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByCallback", astYuvGetValGet, sizeof(astYuvGetValGet) / sizeof(StringValueGet_s));

	if (!stYuvGetModParam.bEnabled)
	{
		INFO_PRINTF("Yuv Get Module Not Enabled!\n");	
		return NULL;
	}
	
	memset(&stYuvCreate, 0, sizeof(MaQueYuvCreate_s));

	stYuvCreate.nWidth = stYuvGetModParam.nCreateWidth;
	stYuvCreate.nHeight = stYuvGetModParam.nCreateHeight;

	if (LibXmMaQue_YUV_create(0, &stYuvCreate)) 
	{
		ERROR_PRINTF("LibXmMaQue_YUV_create err!\n");	
		return NULL;
	}

	INFO_PRINTF("Create OK, Width: %d, Height: %d.\n", stYuvCreate.nWidth, stYuvCreate.nHeight);

	memset(&stYuvParam, 0, sizeof(MaQueYuvParam_s));

	stYuvParam.nWidth = stYuvGetModParam.nCfgWidth;
	stYuvParam.nHeight = stYuvGetModParam.nCfgHeight;

	stCallbackArg.pFrameCount = &nFrames;
	stCallbackArg.bWriteFile = stYuvGetModParam.bWriteFile;
		
	stYuvParam.stCallback.pCallbackFuncPtr = Demo_Yuv_getFrame_callback;
	stYuvParam.stCallback.pCallbackArg = &stCallbackArg;
			
	res = LibXmMaQue_YUV_setParam(0, &stYuvParam);
	if (res != XM_SUCCESS) 
	{
		ERROR_PRINTF("LibXmMaQue_YUV_setParam err!\n");	
		goto ExitHandler;
	}

	if (stYuvGetModParam.nFrames <= 0)
	{
		stYuvGetModParam.nFrames = (XM_U32)-1;
	}

	while(!g_bYuvStop && nFrames <= stYuvGetModParam.nFrames)
	{
		usleep(10*1000);
	}

	stYuvParam.stCallback.pCallbackFuncPtr = NULL;
	LibXmMaQue_YUV_setParam(0, &stYuvParam);

	res = XM_SUCCESS;

ExitHandler:

	LibXmMaQue_YUV_destroy(0);

	return NULL;
}

static XM_S32 Demo_Yuv_getFrameByMalloc(XM_S32 bLibMalloc)
{
	XM_S32 res;
	MaQueYuvCreate_s stYuvCreate;
	MaQueYuvParam_s stYuvParam;
	MaQueYuvFrame_s stYuvFrame;
	XM_S32 nFrames = 0;
	YuvGetModParam_s stYuvGetModParam = { 0 };
	StringValueGet_s astYuvGetValGet[] = 
	{
		{"bEnabled:", &stYuvGetModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.bEnabled)},
		{"bWriteFile:", &stYuvGetModParam.bWriteFile, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.bWriteFile)},
		{"nFrames:", &stYuvGetModParam.nFrames, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nFrames)},	
		{"nCreateWidth:", &stYuvGetModParam.nCreateWidth, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCreateWidth)},	
		{"nCreateHeight:", &stYuvGetModParam.nCreateHeight, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCreateHeight)},
		{"nCfgWidth:", &stYuvGetModParam.nCfgWidth, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCfgWidth)},	
		{"nCfgHeight:", &stYuvGetModParam.nCfgHeight, XM_VALUE_TYPE_INT, sizeof(stYuvGetModParam.nCfgHeight)},
	};

	if (bLibMalloc)
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByLibMalloc", astYuvGetValGet, sizeof(astYuvGetValGet) / sizeof(StringValueGet_s));
	}
	else
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByUserMalloc", astYuvGetValGet, sizeof(astYuvGetValGet) / sizeof(StringValueGet_s));
	}
	
	if (!stYuvGetModParam.bEnabled)
	{
		INFO_PRINTF("Yuv Get Module Not Enabled!\n");	
		return XM_SUCCESS;
	}
	
	memset(&stYuvCreate, 0, sizeof(MaQueYuvCreate_s));

	stYuvCreate.nWidth = stYuvGetModParam.nCreateWidth;
	stYuvCreate.nHeight = stYuvGetModParam.nCreateHeight;

	if (LibXmMaQue_YUV_create(0, &stYuvCreate)) 
	{
		ERROR_PRINTF("LibXmMaQue_YUV_create err!\n");	
		return XM_FAILURE;
	}

	INFO_PRINTF("Create OK, Width: %d, Height: %d.\n", stYuvCreate.nWidth, stYuvCreate.nHeight);
		

	memset(&stYuvParam, 0, sizeof(MaQueYuvParam_s));

	stYuvParam.nWidth = stYuvGetModParam.nCfgWidth;
	stYuvParam.nHeight = stYuvGetModParam.nCfgHeight;
	
	LibXmMaQue_YUV_setParam(0, &stYuvParam);

	memset(&stYuvFrame, 0, sizeof(MaQueYuvFrame_s));

	if (!bLibMalloc)
	{
		stYuvFrame.nBufLen = Demo_Yuv_getBufLen(stYuvParam.nWidth, stYuvParam.nHeight);
		stYuvFrame.pBuffer = malloc(stYuvFrame.nBufLen);
		if (!stYuvFrame.pBuffer)
		{
			ERROR_PRINTF("malloc stYuvFrame.pBuffer Failed: %d <001>!\n", stYuvFrame.nBufLen);	
			res = XM_FAILURE;
			goto ExitHandler;
		}
	}

	if (stYuvGetModParam.nFrames <= 0)
	{
		stYuvGetModParam.nFrames = (XM_U32)-1;
	}

	while (!g_bYuvStop && nFrames < stYuvGetModParam.nFrames)
	{
		if (bLibMalloc)
		{
			stYuvFrame.nBufLen = 0;
			stYuvFrame.pBuffer = NULL;
		}
		
		res = LibXmMaQue_YUV_getFrame(0, &stYuvFrame);
		if (XM_SUCCESS == res)
		{
			nFrames++;

			if (!stYuvGetModParam.bWriteFile)
			{
				INFO_PRINTF("LibXmMaQue_YUV_getFrame OK, nFrames: %d!\n", nFrames);	
				Demo_Yuv_printFrame("Demo_Yuv_getFrameByMalloc() <001>", &stYuvFrame);
			}
			else
			{
				XM_CHAR aFileName[256];
		
				res = Demo_Yuv_makeFilePath(aFileName, bLibMalloc ? YUV_BUFFER_TYPE_LIB_MALLOC : YUV_BUFFER_TYPE_USER_MALLOC, 
							nFrames, stYuvFrame.nWidth, stYuvFrame.nHeight, stYuvFrame.ePixelFormat);
				if (res != XM_SUCCESS)
				{
					ERROR_PRINTF("Demo_Yuv_makeFilePath Failed!\n");
					return XM_FAILURE;
				}

				MaQue_Demo_File_writeSimply(aFileName, stYuvFrame.pBuffer, stYuvFrame.nDataLen);
			}

			if (bLibMalloc && stYuvFrame.pBuffer && stYuvFrame.nBufLen > 0)
			{
				MaQue_Demo_Mem_release(stYuvFrame.handleMem);
			}
		}
		else
		{
			ERROR_PRINTF("LibXmMaQue_YUV_getFrame err <001>!\n");	
			res = XM_FAILURE;
			goto ExitHandler;
		}	
	}

	if (!bLibMalloc)
	{
		free(stYuvFrame.pBuffer);
	}
	
	res = XM_SUCCESS;

ExitHandler:
	
	LibXmMaQue_YUV_destroy(0);

	return res;
}

static XM_VOID *Demo_Yuv_getFrameByMalloc_task(XM_VOID *pArg)
{
	XM_S32 bLibMalloc = (XM_S32)pArg;

	INFO_PRINTF("bLibMalloc: %d!\n", bLibMalloc);	
	
	Demo_Yuv_getFrameByMalloc(bLibMalloc);

	return NULL;
}

static XM_S32 Demo_Yuv_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "stop", strlen("stop")))
	{
		g_bYuvStop = 1;
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Yuv_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Yuv_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByCallback", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Yuv_getFrameByCallback_task, NULL, "Yuv_ByCallback");
		}
	}
	
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByLibMalloc", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Yuv_getFrameByMalloc_task, (XM_VOID *)1, "Yuv_LibMalloc");
		}
	}

	{
		bEnabled = 0;
		
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"GetYuvByUserMalloc", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Yuv_getFrameByMalloc_task, (XM_VOID *)0, "Yuv_UsrMalloc");
		}
	}

	return XM_SUCCESS;
}




