

#ifndef MAQUE_AAC_H
#define MAQUE_AAC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 *	����: 	AAC�����ʼ��
 * 	���Σ�	[in]buffer_max_len ���δ������󳤶�
 *  ���أ�	0 �ɹ�
 *******************************************************************/
XM_S32 LibXmMaQue_AacDec_init(XM_S32 bufLenMax);

/*******************************************************************
 *	����: 	AAC��������
 *  ���أ�	0 �ɹ�
 *******************************************************************/
XM_S32 LibXmMaQue_AacDec_destroy(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MEM_H


