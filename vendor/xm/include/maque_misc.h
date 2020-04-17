
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_misc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: �������Ӳ�����ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/

#ifndef MAQUE_MISC_H
#define MAQUE_MISC_H


#ifdef __cplusplus
extern "C" {
#endif

/// ��ʼ����λ������������ָʾ�ơ�
/// 
/// \param [in] �ޡ�
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
XM_S32 LibXmMaQue_Misc_ResetLedInit(XM_VOID);

/// ���ƺ�������ɫLed״̬��������û�к������ƣ���Ҫ���ø�API��
/// 
/// \param [in] red ��ɫLed 1:������0:Ϩ��
/// \param [in] green ��ɫLed 1:������0:Ϩ��
/// \param [in] blue ��ɫLed 1:������0:Ϩ��
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
XM_S32 LibXmMaQue_Misc_setLedState(XM_S32 red, XM_S32 green, XM_S32 blue);

/// ��ȡReset��״̬��
/// 
/// \param [out] pValue ��ȡreset����״̬ 1 : reset����û��		0 : reset���¡�
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��

XM_S32 LibXmMaQue_Misc_readResetKey(XM_U32 *pValue);


/*
������������
	ϵͳ����ָʾ�Ʋ�����
��������
	  ��������				����
	bOpen [in]		�Ƿ��ϵͳ����ָʾ�ƣ�1:������0:�رա�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�������û��ϵͳ����ָʾ�ƣ���Ҫ���ø�API��
*/
XM_S32 LibXmMaQue_Misc_openSystemLed(XM_S32 bOpen);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MISC_H

