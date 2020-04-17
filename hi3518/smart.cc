#include "smart.h"
#include <spdlog/spdlog.h>
extern "C"
{
#include <maque_type.h>
#include <maque_smart.h>
#include <maque_osd.h>
}

#define DIV_UP(x, a)             ( ((x) + ((a) - 1) ) / a )

XM_S32 MaQue_JpegEnc_getFrame_callback (XM_VOID *pUserArg, MaQueSmartJpegFrame_s *pstJpegFrame)
{
	XM_CHAR acFile[256]  = {0};
	FILE *pFile;
	static XM_S32 jpeg_cnt = 0;

	printf("aClassName[%s] idx = %d, toltal = %d\n", pstJpegFrame->aClassName, pstJpegFrame->nIndex, pstJpegFrame->nToltalJpeg);
	
	if (jpeg_cnt < 30000)
	{
		sprintf(acFile, "ai/snap_%d.jpg", jpeg_cnt);
		pFile = fopen(acFile, "wb");
		if (pFile == NULL)
		{
			spdlog::error("open file err");
			return XM_FAILURE;
		}

		fwrite(pstJpegFrame->pBuffer, pstJpegFrame->nDataLen, 1, pFile);
		fflush(pFile);
		
		fclose(pFile);
		jpeg_cnt++;
	}
		
	return XM_SUCCESS;
}

XM_S32 MaQue_Draw_OsdAreaRect(MaQueStreamChannel_e eStreamChn, XM_S32 enable, XM_S32 index, XM_S32 thick, XM_S32 x, XM_S32 y, XM_S32 width, XM_S32 height)
{
	XM_S32 res;
	MaQueOsdTitleParam_s stParam;
	XM_S32 len;
	XM_U8 *p = NULL;
	XM_S32 i, j, k;
	
	width = (width >> 2) << 2;
	height = (height >> 1) << 1;

	stParam.index = index;
	stParam.enable = enable;
	stParam.bg_color = 0x00000000;
	stParam.fg_color = 0xFFFF0000;
	stParam.eColorFormat = MAQUE_COLOR_FMT_RGB2BPP;
	stParam.x = x;
	stParam.y = y;
	stParam.width = width;
	stParam.height = height;

	len = stParam.width * stParam.height;

	if (stParam.enable)
	{
		stParam.pPixel = (XM_U8 *)malloc(len/4);
		memset(stParam.pPixel, 0x0, len/4);

		p = (XM_U8 *) stParam.pPixel;

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width/4; j++)
			{
				for (k = 0; k < 4; k++)
				{
					if ((0 == (i/thick)) || (i >= (height - thick))
						|| (0 == j && k < thick) || ((j == width/4 - 1) && (k >= 4 - thick)))
					{
						p[width/4*i + j] |= 0x3 << 2*k;
					}
				}
			}
		}
	}
	else
	{
		stParam.pPixel = NULL;
	}

	LibXmMaQue_OSD_setRGB(0, eStreamChn, &stParam);

	if (stParam.pPixel)
	{
		free(stParam.pPixel);
		stParam.pPixel = NULL;
	}
	
	return XM_SUCCESS;
}

void maq_smart_task_entry(void *pArg)
{
	XM_S32 res;
	MaQueStreamChannel_e eStreamChn;
	MaQueSmartParams_s stMaQueSmartParams;
	MaQueSmartTarget_s stMaQueSmartTarget;
	MaQueSmartLowBitRate_s stMaQueSmartLowBitRate;
	XM_S32 bAdvanceIspEnable = 1;
	XM_FLOAT pdThreshold = 0.75;
	XM_FLOAT fdThreshold = 0.63;
	XM_U32 mbSize = 4;
    XM_S32 mbRangeX, mbRangeY;
    XM_S32 i, j, idx;
	MaQueSmartJpegCallbackParam_s stCallback;
	XM_S32 thick = 4;
	XM_S32 x, y;
	XM_S32 width, height;
	XM_U32 cifWidth = 352, tarWidth = 1920;
	XM_U32 cifHeight = 288, tarHeight = 1080;
	XM_U32 osdFlags[MAQUE_STREAM_CHN_NR] = {0, 0};

	// 目前业务都是并行的，此处的delay 仅是确保图像业务已经开启完成。
	sleep(5);
	
	res = LibXmMaQue_SmartCreate(0);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartCreate() Failed!");
		return;
	}

	stMaQueSmartParams.bEnabled = 1;
	stMaQueSmartParams.pdThreshold = pdThreshold;
	stMaQueSmartParams.fdThreshold = fdThreshold;

	stMaQueSmartParams.bRoiEnabled = 0;
	stMaQueSmartParams.roiThreshold = 256;

	stMaQueSmartParams.roiMbMode = MAQUE_SMART_ROI_MB_MODE_16X16;
	stMaQueSmartParams.imgWidth = 640;
	stMaQueSmartParams.imgHeight = 360;
	stMaQueSmartParams.pMbMap = NULL;

	switch (stMaQueSmartParams.roiMbMode)
	{
		case MAQUE_SMART_ROI_MB_MODE_4X4:
			mbSize = 4;
			break;
		case MAQUE_SMART_ROI_MB_MODE_8X8:
			mbSize = 8;
			break;
		case MAQUE_SMART_ROI_MB_MODE_16X16:
			mbSize = 16;
			break;
		default:
			return;
	}

	mbRangeX = DIV_UP(stMaQueSmartParams.imgWidth, mbSize);
	mbRangeY = DIV_UP(stMaQueSmartParams.imgHeight, mbSize);
	stMaQueSmartParams.pMbMap = (XM_U8*)malloc(mbRangeX * mbRangeY);
	if (stMaQueSmartParams.pMbMap == NULL)
	{
		return;
	}

	/*****************************************************************************
	1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1
	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1
	********************************************************************************/
	for (i = 0; i < mbRangeY; i++)
	{
		idx = i * mbRangeX;
		for (j = 0; j < mbRangeX; j++)
		{
			if ((j > mbRangeX * i / mbRangeY - mbRangeX / 5) &&
				(j < mbRangeX * i / mbRangeY + mbRangeX / 5))
			{
				stMaQueSmartParams.pMbMap[idx] = 1;
			}
			else
			{
				stMaQueSmartParams.pMbMap[idx] = 0;
			}
			idx++;
		}
	}

	res = LibXmMaQue_SmartSetParameter(0, &stMaQueSmartParams);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartSetParameter() Failed!");
		free(stMaQueSmartParams.pMbMap);
		stMaQueSmartParams.pMbMap = NULL;
		return;
	}

	if (stMaQueSmartParams.pMbMap)
	{
		free(stMaQueSmartParams.pMbMap);
		stMaQueSmartParams.pMbMap = NULL;
	}

	eStreamChn = MAQUE_STREAM_CHN_MAIN;
	stMaQueSmartLowBitRate.bLowBitrate = 1;
	stMaQueSmartLowBitRate.bIsoAdaptive = 1;
	res = LibXmMaQue_SmartLowBitRate(0, eStreamChn, &stMaQueSmartLowBitRate);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartLowBitRate() Failed!");
		return;
	}

	res = LibXmMaQue_SmartAdvanceIsp(0, bAdvanceIspEnable);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartAdvanceIsp() Failed!");
		return;
	}

	res = LibXmMaQue_SmartPDThreshold(0, pdThreshold);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartPDThreshold() Failed!");
		return;
	}

	res = LibXmMaQue_SmartFDThreshold(0, fdThreshold);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartFDThreshold() Failed!");
		return;
	}
	
	stCallback.eType = MAQUE_SMART_JPEG_ENCODE_LARGE;//MAQUE_SMART_JPEG_ENCODE_CUTOUT;
	stCallback.eClass = MAQUE_SMART_JPEG_CLASS_NONE;//MAQUE_SMART_JPEG_CLASS_FD;
	stCallback.stCallback.pCallbackArg = NULL;
	stCallback.stCallback.pCallbackFuncPtr = MaQue_JpegEnc_getFrame_callback;
	LibXmMaQue_SmartRegisterCallback(0, &stCallback);

	while (1)
	{
		res = LibXmMaQue_SmartGetTarget(0, &stMaQueSmartTarget);
		if (XM_SUCCESS != res)
		{
			spdlog::error("LibXmMaQue_SmartGetTarget() Failed!");
			return;
		}

		idx = 2;

		for (i = MAQUE_MAX_CLASS_NUM * MAQUE_MAX_RECT_NUM; i > 0; i--)
		{
			if (osdFlags[MAQUE_STREAM_CHN_MAIN] & (0x1 << i))
			{
				MaQue_Draw_OsdAreaRect(MAQUE_STREAM_CHN_MAIN, 0, i, thick, 0, 0, 4, 4);
				osdFlags[MAQUE_STREAM_CHN_MAIN] &= ~(0x1 << i);
			}
		}

		if (stMaQueSmartTarget.targetPDNum > 0)
		{
			printf("\n\nPeople Detection : TotalNum %d\n", stMaQueSmartTarget.targetPDNum);

			for (i = 0; i < stMaQueSmartTarget.targetPDNum; i++)
			{
				printf("(%d, %d), (%d, %d)\n", stMaQueSmartTarget.aPDRect[i].s16X1, stMaQueSmartTarget.aPDRect[i].s16Y1,
						stMaQueSmartTarget.aPDRect[i].s16X2, stMaQueSmartTarget.aPDRect[i].s16Y2);
			}
		}

		if (stMaQueSmartTarget.targetFDNum > 0)
		{
			printf("\n\nFace Detection : TotalNum %d\n", stMaQueSmartTarget.targetFDNum);

			for (i = 0; i < stMaQueSmartTarget.targetFDNum; i++)
			{
				printf("(%d, %d), (%d, %d)\n", stMaQueSmartTarget.aFDRect[i].s16X1, stMaQueSmartTarget.aFDRect[i].s16Y1,
						stMaQueSmartTarget.aFDRect[i].s16X2, stMaQueSmartTarget.aFDRect[i].s16Y2);

				width = (stMaQueSmartTarget.aFDRect[i].s16X2-stMaQueSmartTarget.aFDRect[i].s16X1)*tarWidth/stMaQueSmartParams.imgWidth;
				height = (stMaQueSmartTarget.aFDRect[i].s16Y2-stMaQueSmartTarget.aFDRect[i].s16Y1)*tarHeight/stMaQueSmartParams.imgHeight;

				width = (width >> 2) << 2;
				height = (height >> 1) << 1;

				x = stMaQueSmartTarget.aFDRect[i].s16X1*cifWidth/stMaQueSmartParams.imgWidth;
				y = stMaQueSmartTarget.aFDRect[i].s16Y1*cifHeight/stMaQueSmartParams.imgHeight;
				
				MaQue_Draw_OsdAreaRect(MAQUE_STREAM_CHN_MAIN, 1, idx, thick, x, y, width, height);
				osdFlags[MAQUE_STREAM_CHN_MAIN] |= (0x1 << idx);
				idx ++;
			}
		}
		
		usleep(25*1000);
	}

	res = LibXmMaQue_SmartDestory(0);
	if (XM_SUCCESS != res)
	{
		spdlog::error("LibXmMaQue_SmartDestory() Failed!\n");
		return;
	}
}