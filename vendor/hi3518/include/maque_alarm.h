
#ifndef MAQUE_ALARM_H
#define MAQUE_ALARM_H


#ifdef __cplusplus
extern "C" {
#endif


/*
【功能描述】
	获取实际报警输入数。
【参数】
	无。
【返回值】
	报警输入数。
*/
XM_S32 LibXmMaQue_AlarmIn_getCount(XM_VOID);

/*
【功能描述】
	获取索引index对应的报警输入状态。
【参数】
	 参数名称			描述
	index	[in]	报警输入通道索引，从0开始，须小于LibXmMaQue_AlarmIn_getCount()返回值。
	pbEnabled [out]	报警输入状态值，1:有报警输入；0:无报警输入。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_AlarmIn_getStatus(XM_S32 index, XM_S32 *pbEnabled);

/*
【功能描述】
	获取实际报警输出数。
【参数】
	无。
【返回值】
	报警输出数。
*/
XM_S32 LibXmMaQue_AlarmOut_getCount(XM_VOID);

/*
【功能描述】
	设置索引index对应的报警输出状态。
【参数】
	 参数名称			描述
	index	[in]	报警输出通道索引，从0开始，须小于LibXmMaQue_AlarmOut_getCount()返回值。
	bEnabled [in]	报警输出使能值，1:使能报警输出；0:停止报警输出。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/

XM_S32 LibXmMaQue_AlarmOut_enable(XM_S32 index, XM_S32 bEnabled);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_ALARM_H

