
#ifndef MAQUE_MP4_H
#define MAQUE_MP4_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
 *	功能: 	MP4录制打开
 *  返回：	0 成功
 ps:MP4录制启动条件:1.配置音频参数
 					2.调用H264视频获取接口
 					3.调用AAC音频获取接口
 *******************************************************************/
XM_S32 LibXmMaQue_Mp4_start(XM_CHAR *pFileName);

/*******************************************************************
 *	功能: 	MP4录制停止
 *  返回：	0 成功
 *******************************************************************/
XM_S32 LibXmMaQue_Mp4_stop(XM_VOID);

#ifdef __cplusplus
}
#endif


#endif


