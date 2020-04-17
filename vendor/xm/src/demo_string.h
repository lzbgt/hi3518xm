
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_string.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 声明演示程序用到的字符串操作相关接口函数。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_STRING_H
#define MAQUE_DEMO_STRING_H

typedef struct 
{
	XM_U32 enumValue;  /* 枚举值。*/
	XM_CHAR *pStr;  /* 枚举值对应的名称。*/
} EnumStr_s;

/*
输入参数：pStr：需要转换成枚举（Enum）的字符串。
	pstStrEnum：包含所有支持的枚举值和对应值名的数组。参考：">关键结构体>>EnumStr_s"。
	nEntries：pstStrEnum中包含的结构体条数。
输出参数：无。
返回：成功：枚举值；失败：nEntries。
函数功能：在pstStrEnum（数组）中查找与传入参数pStr匹配的枚举值名，如果找到，就返回该枚举值，
	否则返回nEntries（它必须是不在有效枚举值范围内）。
*/
XM_S32 MaQue_Demo_String_toEnum(XM_CHAR *pStr, EnumStr_s *pstStrEnum, XM_S32 nEntries);

/*
输入参数：enumValue：需要转换成值名的枚举（Enum）值。
	 pstStrEnum：包含所有支持的枚举值和对应值名的数组。参考：">关键结构体>>EnumStr_s"。
	 nEntries：pstStrEnum中包含的结构体条数。
输出参数：无。
返回：成功：枚举值名（字符串）；失败："NULL"。
函数功能：在pstStrEnum（数组）中查找与传入参数enumValue匹配的枚举值，如果找到，
	就返回该枚举值名，否则返回"NULL"。
*/
XM_CHAR *MaQue_Demo_String_enum2Str(XM_U32 enumValue, EnumStr_s *pstStrEnum, XM_S32 nEntries);


XM_CHAR *MaQue_Demo_String_trimLeftSpace(XM_CHAR *pStr);
XM_CHAR *MaQue_Demo_String_trimRightSpace(XM_CHAR *pStr);
XM_CHAR *MaQue_Demo_String_trimLeftRightSpace(XM_CHAR *pStr);

#endif

