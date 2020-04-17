
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_env.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示Env分区读写操作。
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
#include "demo_env.h"


static XM_CHAR *g_apEnvKeys[] =
{
	"bootargs",
	"bootdelay",
	"baudrate",
	"serverip",
	"ipaddr",
	"netmask",
	"ethaddr",
	"osmem",
	NULL,
};

static XM_S32 Demo_Env_setKeyValue(XM_CHAR *pKey, XM_CHAR *pValue)
{
	XM_S32 res;
	
	INFO_PRINTF("pKey: %s, pValue: %s.\n", pKey, pValue);
	
	res = LibXmMaQue_Env_setKeyValue(pKey, pValue);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_Env_setKeyValue() Failed, pKey: %s, pValue: %s!\n", pKey, pValue);
		return XM_FAILURE;
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_Env_writeKeyValue(XM_CHAR *pKey, XM_CHAR *pValue)
{
	XM_S32 res;
	
	INFO_PRINTF("pKey: %s, pValue: %s.\n", pKey, pValue);
	
	res = LibXmMaQue_Env_writeKeyValue(pKey, pValue);
	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_Env_writeKeyValue() Failed, pKey: %s, pValue: %s!\n", pKey, pValue);
		return XM_FAILURE;
	}

	return XM_SUCCESS;
}

static XM_S32 Demo_Env_getKeyValue(XM_CHAR *pKey, XM_CHAR *pValue, XM_S32 valLen)
{
	XM_S32 res;
	
	res = LibXmMaQue_Env_getValue(pKey, pValue, valLen);

	if (res != XM_SUCCESS)
	{
		ERROR_PRINTF("LibXmMaQue_Env_getValue() Failed, pKey: %s!\n", pKey);
		return XM_FAILURE;
	}

	INFO_PRINTF("pKey: %s, pValue: %s.\n", pKey, pValue);

	return XM_SUCCESS;
}

static XM_VOID XmEnv_showKeyValue(XM_CHAR *pKey)
{
	XM_S32 res;
	XM_CHAR aVal[256];

	memset(aVal, 0x0, sizeof(aVal));
	
	res = LibXmMaQue_Env_getValue(pKey, aVal, sizeof(aVal) - 1);
	if(res >= 0 && aVal[0] != 0 )
	{
		SIMPLE_PRINTF("%s = %s\n", pKey, aVal);
	}
	
	return;
}

static XM_VOID Demo_Env_showAllKeyValues(XM_VOID)
{
	XM_S32 index = 0;
	
	while (g_apEnvKeys[index])
	{
		XmEnv_showKeyValue(g_apEnvKeys[index++]);
	}
	
	return;
}

XM_S32 MaQue_Demo_Env_start(XM_VOID)
{
	XM_CHAR aEthAddrOrg[32];
	XM_CHAR aEthAddrCur[32];
	XM_S32 bEnabled = 0;
	StringValueGet_s astEnabledValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
	};

	MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"EnvTest", astEnabledValGet, sizeof(astEnabledValGet) / sizeof(StringValueGet_s));

	INFO_PRINTF("EnvTest bEnabled: %d\n", bEnabled);

	if (!bEnabled)
	{
		INFO_PRINTF("EnvTest bEnabled: %d\n", bEnabled);
		return XM_FAILURE;
	}
	
	Demo_Env_showAllKeyValues();

	Demo_Env_getKeyValue("ethaddr", aEthAddrOrg, sizeof(aEthAddrOrg));
	
	Demo_Env_writeKeyValue("ethaddr", "00:01:23:45:67:89");

	Demo_Env_getKeyValue("ethaddr", aEthAddrCur, sizeof(aEthAddrCur));

	Demo_Env_writeKeyValue("ethaddr", aEthAddrOrg);

	Demo_Env_getKeyValue("ethaddr", aEthAddrCur, sizeof(aEthAddrCur));

	Demo_Env_setKeyValue("ethaddr", "00:01:23:45:67:89");
	Demo_Env_setKeyValue("serverip", "192.168.1.100");
	LibXmMaQue_Env_save();  /* 一次性写到flash等存储设备中。*/

	Demo_Env_showAllKeyValues();

	return XM_SUCCESS;
}

