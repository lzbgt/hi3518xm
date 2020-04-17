
#ifndef MAQUE_ALARM_H
#define MAQUE_ALARM_H


#ifdef __cplusplus
extern "C" {
#endif


/*
������������
	��ȡʵ�ʱ�����������
��������
	�ޡ�
������ֵ��
	������������
*/
XM_S32 LibXmMaQue_AlarmIn_getCount(XM_VOID);

/*
������������
	��ȡ����index��Ӧ�ı�������״̬��
��������
	 ��������			����
	index	[in]	��������ͨ����������0��ʼ����С��LibXmMaQue_AlarmIn_getCount()����ֵ��
	pbEnabled [out]	��������״ֵ̬��1:�б������룻0:�ޱ������롣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_AlarmIn_getStatus(XM_S32 index, XM_S32 *pbEnabled);

/*
������������
	��ȡʵ�ʱ����������
��������
	�ޡ�
������ֵ��
	�����������
*/
XM_S32 LibXmMaQue_AlarmOut_getCount(XM_VOID);

/*
������������
	��������index��Ӧ�ı������״̬��
��������
	 ��������			����
	index	[in]	�������ͨ����������0��ʼ����С��LibXmMaQue_AlarmOut_getCount()����ֵ��
	bEnabled [in]	�������ʹ��ֵ��1:ʹ�ܱ��������0:ֹͣ���������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/

XM_S32 LibXmMaQue_AlarmOut_enable(XM_S32 index, XM_S32 bEnabled);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_ALARM_H

