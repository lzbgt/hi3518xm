
#ifndef __MAQUE_MTDVER__
#define __MAQUE_MTDVER__

/*
������������
	��ȡuboot��kernel��rootfs �汾��
��������
	  ��������			����
	pDate [out]	������uboot��kernel��rootfs �汾��
	pNum [out]  �������������
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ýӿ���ϵͳ������أ���ͨ�á�
*/
XM_S32 LibXmMaQue_Mtd_getMtdVersion(XM_U32 *pDate, XM_S32 *pNum);

/*
������������
	����SN ���кš�
��������
	  ��������			����
	pBuf	[in]	SN ���кš�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ýӿ���ϵͳ������أ���ͨ�á�
*/
XM_S32 LibXmMaQue_Mtd_setIMEI(XM_U8 *pBuf);

/*
������������
	��ȡSN ���кš�
��������
	  ��������			����
	pBuf	[in]	SN ���кš�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ýӿ���ϵͳ������أ���ͨ�á�
*/
XM_S32 LibXmMaQue_Mtd_getIMEI(XM_U8 *pBuf);

#endif

