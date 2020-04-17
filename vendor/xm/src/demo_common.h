/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_common.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 声明演示程序共用函数。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_COMMON_H
#define MAQUE_DEMO_COMMON_H


XM_U64 MaQue_Demo_Common_getNowMs(XM_VOID);
XM_VOID MaQue_Demo_Common_setRefFile(XM_CHAR *pRefFile);
XM_CHAR *MaQue_Demo_Common_getRefFile(XM_VOID);
XM_S32 MaQue_Demo_Common_createThread(VoidP_VoidP_FuncPtr pThreadBodyFuncPtr, XM_VOID *pArg, XM_CHAR *pOwner);


#endif

