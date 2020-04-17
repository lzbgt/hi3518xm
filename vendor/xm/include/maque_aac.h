

#ifndef MAQUE_AAC_H
#define MAQUE_AAC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 *	功能: 	AAC解码初始化
 * 	传参：	[in]buffer_max_len 单次传入的最大长度
 *  返回：	0 成功
 *******************************************************************/
XM_S32 LibXmMaQue_AacDec_init(XM_S32 bufLenMax);

/*******************************************************************
 *	功能: 	AAC解码销毁
 *  返回：	0 成功
 *******************************************************************/
XM_S32 LibXmMaQue_AacDec_destroy(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MEM_H


