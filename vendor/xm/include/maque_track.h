
#ifndef MAQUE_TRACK_H
#define MAQUE_TRACK_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_U32 id;						/**< ID */
	MaQueRectCoord_s stRectCoord;		/**< 区域 */
} MaQueIAObjInfo_s;


typedef XM_S32 (*TrackCallbackFuncPtr )(XM_VOID *pUserArg, MaQueIAObjInfo_s *pstTrackRect);

/*******************************************************************
 *	功能:创建物体跟踪
 *	_validrect:返回的等效检测区域(缩放的主码流)
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_create(MaQueSize_s *pstValidSize);

/*******************************************************************
 *	功能:销毁物体跟踪
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_destroy(XM_VOID);

/*******************************************************************
 *	功能:	使能物体跟踪
 *
 *	ucMinLastFrm: 目标连续存在的最小帧数限制 ,越小检测
 *	时间越短
 *	
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_enable(XM_S32 enable);

/*******************************************************************
 *	功能:	关闭物体跟踪
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_disable(XM_VOID);

/*******************************************************************
 *	功能:	继续物体跟踪
 *
 *	minObjFrames: 目标连续存在的最小帧数限制 ,越小检测
 *	时间越短
 *	
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_continue(XM_U8 minObjFrames);


/*******************************************************************
 *	功能:	物体跟踪回调,回调中显示移动区域在等效检测区域中的坐标
 * 	返回：	0 成功
 * 			<0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Track_register(TrackCallbackFuncPtr pCallbackFuncPtr, XM_VOID *pCallbackArg);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_TRACK_H

