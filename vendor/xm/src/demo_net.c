
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_net.c
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ�������ù��ܡ�
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
#include "demo_net.h"


typedef struct
{
	XM_S32 bEnabled;
	XM_CHAR aEthName[16];
	XM_CHAR aIpAddr[16];
	XM_CHAR aNetMask[16];
	XM_CHAR aGateway[16];
} NetCfgModParam_s;

static XM_VOID Demo_NetCfg_ModParam_print(XM_CHAR *pOwner, NetCfgModParam_s *pstNetCfgModParam)
{
	SIMPLE_PRINTF("@%s->Demo_NetCfg_ModParam_print:\n", pOwner);
	SIMPLE_PRINTF("\taEthName: %s\n", pstNetCfgModParam->aEthName);
	SIMPLE_PRINTF("\taIpAddr: %s\n", pstNetCfgModParam->aIpAddr);
	SIMPLE_PRINTF("\taNetMask: %s\n", pstNetCfgModParam->aNetMask);
	SIMPLE_PRINTF("\taGateway: %s\n", pstNetCfgModParam->aGateway);
}

XM_S32 MaQue_Demo_Net_init(XM_VOID)
{
	XM_CHAR aMac[18] = { 0 };
	NetCfgModParam_s stNetCfgModParam = { 0 };
	StringValueGet_s astOsdTitleValGet[] = 
	{
		{"bEnabled:", &stNetCfgModParam.bEnabled, XM_VALUE_TYPE_INT, sizeof(stNetCfgModParam.bEnabled)},
		{"EthName:", &stNetCfgModParam.aEthName, XM_VALUE_TYPE_STRING, sizeof(stNetCfgModParam.aEthName)},	
		{"IpAddr:", &stNetCfgModParam.aIpAddr, XM_VALUE_TYPE_STRING, sizeof(stNetCfgModParam.aIpAddr)},	
		{"NetMask:", &stNetCfgModParam.aNetMask, XM_VALUE_TYPE_STRING, sizeof(stNetCfgModParam.aNetMask)},
		{"Gateway:", &stNetCfgModParam.aGateway, XM_VALUE_TYPE_STRING, sizeof(stNetCfgModParam.aGateway)},
	};
	

	INFO_PRINTF("<Enter>\n");

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"NetConfig", astOsdTitleValGet, sizeof(astOsdTitleValGet) / sizeof(StringValueGet_s));

	INFO_PRINTF("NetConfig bEnabled: %d\n", stNetCfgModParam.bEnabled);

	if (!stNetCfgModParam.bEnabled)
	{
		INFO_PRINTF("NetConfig bEnabled: %d\n", stNetCfgModParam.bEnabled);
		return XM_FAILURE;
	}

	Demo_NetCfg_ModParam_print("MaQue_Demo_Net_init()", &stNetCfgModParam);
	
	LibXmMaQue_Net_getMac(stNetCfgModParam.aEthName, aMac, sizeof(aMac));

	INFO_PRINTF("aMac: %s\n", aMac);
	
	LibXmMaQue_Net_setHostIP(stNetCfgModParam.aEthName, stNetCfgModParam.aIpAddr, stNetCfgModParam.aNetMask, stNetCfgModParam.aGateway);

	return XM_SUCCESS;
}


