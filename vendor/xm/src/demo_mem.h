
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_mem.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 演示内存分配和释放函数注册。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_MEM_H
#define MAQUE_DEMO_MEM_H


XM_S32 MaQue_Demo_Mem_alloc(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam);
XM_S32 MaQue_Demo_Mem_release(XM_HANDLE handle);
XM_S32 MaQue_Demo_Mem_setLength(XM_HANDLE handle, XM_U32 len);
XM_S32 MaQue_Demo_Mem_addRef(XM_HANDLE handle);

#endif

