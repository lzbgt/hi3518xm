
#ifndef MAQUE_ASR_H
#define MAQUE_ASR_H


#ifdef __cplusplus
extern "C" {
#endif

typedef XM_S32(*ASRGetResultFuncPtr)(XM_VOID *pUserArg, XM_VOID *ASRData);

/*******************************************************************
 *	����:	��������ʶ����
 * 	���أ�	0 �ɹ�
 * 			<0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_start(XM_VOID);

/*******************************************************************
 *	����:	ע������ʶ��״̬��ȡ�ص�
 * 	���أ�	0 �ɹ�
 * 			<0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_register(ASRGetResultFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*******************************************************************
 *	����:	ֹͣ����ʶ����
 * 	���أ�	0 �ɹ�
 * 			<0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_stop(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_ASR_H

