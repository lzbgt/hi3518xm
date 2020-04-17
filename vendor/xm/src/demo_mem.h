
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_mem.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ��ʾ�ڴ������ͷź���ע�ᡣ
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_MEM_H
#define MAQUE_DEMO_MEM_H


XM_S32 MaQue_Demo_Mem_alloc(XM_HANDLE *pHandle, MaQueMemAllocParam_s *pstAllocParam);
XM_S32 MaQue_Demo_Mem_release(XM_HANDLE handle);
XM_S32 MaQue_Demo_Mem_setLength(XM_HANDLE handle, XM_U32 len);
XM_S32 MaQue_Demo_Mem_addRef(XM_HANDLE handle);

#endif

