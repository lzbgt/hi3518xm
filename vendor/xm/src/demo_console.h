
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_console.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: �ṩ����̨��ؽӿڣ����û�����ʾ�����У��������̡�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
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

