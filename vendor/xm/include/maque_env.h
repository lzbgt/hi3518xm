
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_env.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����Env������д��ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_ENV_H
#define MAQUE_ENV_H


#ifdef __cplusplus
extern "C" {
#endif

/*
������������
	����Env�����ļ�ֵ��Ϊ���ύЧ�ʣ�ϵͳ����ʱ�����Env���������ڴ�����Ϊ���ݡ���������
	�ʹ��ڴ汸������ȡ��д�������ȰѼ�ֵ�����õ��ڴ汸����������CRC32У�����д��Flash
	�С����Env����������ͬ�ļ���������ɾ��ԭ�еģ����ں�������µġ�
	��APIֻ�ǰѼ�ֵ�����õ��ڴ�ı������У���û��ʵ��д��Flash�С�д��Flash�Ĳ�������
	����API��LibXmMaQue_Env_save()��ɡ�������API��LibXmMaQue_Env_writeKeyValue()��ȣ�
	�����൱�ڼ�����LibXmMaQue_Env_setKeyValue()��LibXmMaQue_Env_save()���ܣ���������
	�޸��ڴ汸������ͬʱ����д��Flash��ע��������API�������ṩ��API�ĺô��ǣ����ж�
	����ֵ����Ҫ����ʱ���ȸ��µ��ڴ汸���������һ����д��Flash���ɴ�����Ч�ʣ�ͬʱ
	����Flash��ʹ��������
��������
	  ��������			����
	pKey  [in]		��Ҫ���õļ�����
	pValue [in]		��Ҫ���õļ�ֵ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	����LibXmMaQue_Env_setKeyValue()��ֻ�Ѽ�ֵ�����õ��ڴ汸����������д��Flash Env
	����������������ܵ�LibXmMaQue_Env_getValue()���Ի�ȡ�������ϵ��ᶪʧ��Ҫ�뱣
	�浽Flash�У����Ե���LibXmMaQue_Env_save()����ֱ�ӵ���LibXmMaQue_Env_writeKeyValue()��
*/
XM_S32 LibXmMaQue_Env_setKeyValue(XM_CHAR *pKey, XM_CHAR *pValue);

/*
������������
	��Env�ڴ汸���������ݱ��浽Flash�С�һ�����һ��API��LibXmMaQue_Env_setKeyValue()���ʹ�á�
��������
	�ޡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_Env_save(XM_VOID);

/*
������������
	�Ѻ�������ļ�ֵ�ԣ����µ�Env�ڴ汸������Ȼ�󱣴浽Flash�С����Env�����Ѿ�����ͬ�ļ���
	������ɾ��ԭ�еģ����ں�������µġ�ע�⣺��ǰ��API��LibXmMaQue_Env_setKeyValue()����
	���൱�ڼ�����LibXmMaQue_Env_setKeyValue()��LibXmMaQue_Env_save()���ܡ�
��������
	  ��������			����	
	pKey [in]		��Ҫ���浽Flash�еļ�����
	pValue [in]		��Ҫ���浽Flash�еļ�ֵ��
������ֵ��
	XM_SUCCESS	�ɹ���
	����	ʧ�ܣ���ο������롣
��ע�⡿
	����LibXmMaQue_Env_writeKeyValue()���Ѽ�ֵ�����õ��ڴ汸������ͬʱд��Flash Env������
	Ƶ�����û�Ӱ��Flash������
*/
XM_S32 LibXmMaQue_Env_writeKeyValue(XM_CHAR *pKey, XM_CHAR *pValue);

/*
������������
	��ȡEnv����pKey��Ӧ��ֵ�����Env�����Ҳ���pKey��Ӧ�ļ�ֵ��������ʧ�ܡ�
��������
	  ��������			����
	pKey [out]		��Ҫ��ȡ��ֵ�ļ�����
	pValue [out]	��������ȡ��ֵ��Bufferָ�롣
	valueLen [in]	�����pValue�����ռ䣨��λ���ֽڣ��������ַ����Ľ�������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
��pValue�����BufferҪ�㹻�󣬷����ȡ���ַ������ضϡ�
*/
XM_S32 LibXmMaQue_Env_getValue(XM_CHAR *pKey, XM_CHAR *pValue, XM_S32 valueLen);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_ENV_H

