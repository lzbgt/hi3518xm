
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_string.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 实现演示程序用到的字符串操作相关接口函数。
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
#include "demo_string.h"


XM_S32 MaQue_Demo_String_toEnum(XM_CHAR *pStr, EnumStr_s *pstEnumStr, XM_S32 nEntries)
{
	XM_S32 i;

	for (i = 0; i < nEntries; i++)
	{
		if (!strcasecmp(pstEnumStr[i].pStr, pStr))
		{
			return pstEnumStr[i].enumValue;
		}
	}
	
	ERROR_PRINTF("Invalid pStr:%s.\n", pStr);

	return nEntries;
}

XM_CHAR *MaQue_Demo_String_enum2Str(XM_U32 enumValue, EnumStr_s *pstEnumStr, XM_S32 nEntries)
{
	XM_S32 i;

	for (i = 0; i < nEntries; i++)
	{
		if (enumValue == pstEnumStr[i].enumValue)
		{
			return pstEnumStr[i].pStr;
		}
	}
	
	ERROR_PRINTF("Invalid enumValue: %d.\n", enumValue);

	return "NULL";
}

XM_CHAR *MaQue_Demo_String_trimLeftSpace(XM_CHAR *pStr) 
{
	XM_CHAR *p; 

	if (!pStr)
	{
		return NULL;
	}
	
	p = pStr;
	
	if (0 == strlen(pStr))
	{
		return (pStr);
	}

	while ( *p != 0 && *p >= 0 && *p <= 0x20)
	{
		p++;
	}

	return p;
}

XM_CHAR *MaQue_Demo_String_trimRightSpace(XM_CHAR *pStr)
{       
	XM_CHAR *p;

	if (!pStr)
	{
		return NULL;
	}
	
	if (0 == strlen(pStr))
	{
		return pStr;
	}

	p = pStr + strlen(pStr) - 1;

	while (p >= pStr && (*p >= 0 && *p <= 0x20))
	{
		p--; 
	}

	*(p + 1) = 0;
	
	return pStr;
}      

XM_CHAR *MaQue_Demo_String_trimLeftRightSpace(XM_CHAR *pStr)
{
    return(MaQue_Demo_String_trimRightSpace(MaQue_Demo_String_trimLeftSpace(pStr)));
}

