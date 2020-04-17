
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_console.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 提供控制台相关接口，供用户在演示过程中，控制流程。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_CONSOLE_H
#define MAQUE_DEMO_CONSOLE_H


typedef struct
{
	S32_VoidP_VoidP_FuncPtr pCallbackFuncPtr;
	XM_VOID *pCallbackArg;
} ConsoleCallback_s;

XM_S32 MaQue_Demo_Console_registerCallback(ConsoleCallback_s *pstCallback);
XM_S32 MaQue_Demo_Console_run(XM_VOID);
XM_S32 MaQue_Demo_Console_OnLine(XM_CHAR *pBuf, S32_S32_CharPP_FuncPtr cb);

#endif

