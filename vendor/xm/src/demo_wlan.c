
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_net.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.04.13
 * 功能描述: 演示无线设置功能。
 *
 * 修改历史:  
 ** 1.日期: 2019.04.13
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.04.13
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#include "demo_inc.h"
#include "demo_wlan.h"

static EnumStr_s g_astWpaLinkState[MAQUE_WPA_NR+1] =
{
	{MAQUE_WPA_NOT_EXIST+1, "NotExist"},
	{MAQUE_WPA_UNLINK+1, "UnLink"},
	{MAQUE_WPA_LINK+1, "Link"}
};

XM_CHAR *MaQue_Demo_WpaLinkState_enum2Str(XM_U32 enumValue)
{
	return MaQue_Demo_String_enum2Str(enumValue, g_astWpaLinkState, (MAQUE_WPA_NR+1));
}

static XM_VOID Demo_Wlan_WifiInfo_print(XM_CHAR *pOwner, MaQueWlanSearchInfo_s *pstWlanSearchInfo)
{
	XM_S32 i;
	SIMPLE_PRINTF("@%s->Demo_Wlan_WifiInfo_print:\n", pOwner);
	for (i = 0; i < pstWlanSearchInfo->count; i ++)
	{
		SIMPLE_PRINTF("%d, %s, %d, %d, %d, %d, %02x:%02x:%02x:%02x:%02x:%02x\n", i, \
			pstWlanSearchInfo->aNet[i].aSsid, pstWlanSearchInfo->aNet[i].rssi, pstWlanSearchInfo->aNet[i].auth, \
			pstWlanSearchInfo->aNet[i].eEncryp, pstWlanSearchInfo->aNet[i].chn, pstWlanSearchInfo->aNet[i].aMac[0], \
			pstWlanSearchInfo->aNet[i].aMac[1], pstWlanSearchInfo->aNet[i].aMac[2],\
			pstWlanSearchInfo->aNet[i].aMac[3], pstWlanSearchInfo->aNet[i].aMac[4],\
			pstWlanSearchInfo->aNet[i].aMac[5]);
	}
}

static XM_S32 OnConsoleWlan(XM_S32 argc, XM_CHAR** argv)
{
	XM_S32 res;
	MaQueWlanSearchInfo_s stWlanSearchInfo;
	MaQueWpaLinkState_e eWpaLinkState;
	MaQueConfigWlan_s stWlan;
	
	if (argc < 2)
	{
help:
		printf("***********************************************wlan help***********************************************\n");
		printf("wlan -s			 				:  scan wifi\n");
		printf("wlan -g ethname	 				:  get wpa status\n");
		printf("wlan -a ssid [passwd]			:  config ap mode\n");
		printf("wlan -t ssid encryp [passwd] 	:  config station mode\n");
		printf("***********************************************wlan help***********************************************\n");
		return 0;
	}

	switch (argv[1][1])
	{
		case 's':
			res = LibXmMaQue_Wlan_searchWIFI(&stWlanSearchInfo, 0);
			if (res < 0)
			{
				ERROR_PRINTF("search wifi fail!!!\n");
				return XM_FAILURE;
			}

			Demo_Wlan_WifiInfo_print("MaQue_Demo_Wlan_init",&stWlanSearchInfo);
			
			break;
		case 'g':
			if (argv[2])
			{
				eWpaLinkState = LibXmMaQue_Wlan_getWpaStatus(argv[2]);
				INFO_PRINTF("Current wifi state : %s\n", MaQue_Demo_WpaLinkState_enum2Str(eWpaLinkState+1));
			}
			else
			{
				goto help;
			}
			break;
		case 'a':

			memset(&stWlan, 0x0, sizeof(stWlan));

			if (argc < 3)
			{
				goto help;
			}
			
			strcpy(stWlan.aSsid, argv[2]);

			if (argv[3])
			{
				strcpy(stWlan.aPasswd, argv[3]);
			}
			
			res = LibXmMaQue_Wlan_switch2AP(&stWlan);
			if (res < 0)
			{
				ERROR_PRINTF("switch2ap fail!!!\n");
				return XM_FAILURE;
			}
			break;
		case 't':

			memset(&stWlan, 0x0, sizeof(stWlan));

			if (argc < 4)
			{
				goto help;
			}
			
			strcpy(stWlan.aSsid, argv[2]);

			stWlan.eEncryp = atoi(argv[3]);

			if (argv[4])
			{
				strcpy(stWlan.aPasswd, argv[4]);
			}
			
			res = LibXmMaQue_Wlan_switch2STA(&stWlan);
			if (res < 0)
			{
				ERROR_PRINTF("switch2sta fail!!!\n");
				return XM_FAILURE;
			}
			break;
		default:
			goto help;
			break;
	};
	return 0;
}

static XM_S32 Demo_Wlan_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "wlan", strlen("wlan")))
	{
		MaQue_Demo_Console_OnLine((XM_CHAR*)pData, OnConsoleWlan);
		
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Wlan_init(XM_VOID)
{
	XM_S32 bEnabled = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
		"Wlan", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

	if (bEnabled)
	{
		stCallback.pCallbackArg = NULL;
		stCallback.pCallbackFuncPtr = Demo_Wlan_Console_callback;

		MaQue_Demo_Console_registerCallback(&stCallback);
	}

	return XM_SUCCESS;
}


