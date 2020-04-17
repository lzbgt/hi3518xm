/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_common.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ������ʾ�����ú�����
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_COMMON_H
#define MAQUE_DEMO_COMMON_H


XM_U64 MaQue_Demo_Common_getNowMs(XM_VOID);
XM_VOID MaQue_Demo_Common_setRefFile(XM_CHAR *pRefFile);
XM_CHAR *MaQue_Demo_Common_getRefFile(XM_VOID);
XM_S32 MaQue_Demo_Common_createThread(VoidP_VoidP_FuncPtr pThreadBodyFuncPtr, XM_VOID *pArg, XM_CHAR *pOwner);


#endif

