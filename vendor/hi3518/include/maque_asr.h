
#ifndef MAQUE_ASR_H
#define MAQUE_ASR_H


#ifdef __cplusplus
extern "C" {
#endif

typedef XM_S32(*ASRGetResultFuncPtr)(XM_VOID *pUserArg, XM_VOID *ASRData);

/*******************************************************************
 *	功能:	开启语音识别功能
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_start(XM_VOID);

/*******************************************************************
 *	功能:	注册语音识别状态获取回调
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_register(ASRGetResultFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

/*******************************************************************
 *	功能:	停止语音识别功能
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_ASR_stop(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_ASR_H

