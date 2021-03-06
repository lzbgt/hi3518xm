
#ifndef MAQUE_UVC_H
#define MAQUE_UVC_H


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 *	功能:	创建uvc相关资源
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_UVC_create(XM_VOID);

/*******************************************************************
 *	功能:	销毁uvc相关资源
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_UVC_destroy(XM_VOID);

#ifdef __cplusplus
}
#endif

#endif

