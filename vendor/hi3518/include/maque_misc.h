
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_misc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明混杂操作相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/

#ifndef MAQUE_MISC_H
#define MAQUE_MISC_H


#ifdef __cplusplus
extern "C" {
#endif

/// 初始化复位按键、红蓝绿指示灯。
/// 
/// \param [in] 无。
/// \retval 0  成功
/// \retval <0  失败
XM_S32 LibXmMaQue_Misc_ResetLedInit(XM_VOID);

/// 控制红蓝绿三色Led状态，板子如没有红绿蓝灯，不要调用该API。
/// 
/// \param [in] red 红色Led 1:点亮；0:熄灭。
/// \param [in] green 绿色Led 1:点亮；0:熄灭。
/// \param [in] blue 蓝色Led 1:点亮；0:熄灭。
/// \retval 0  写成功
/// \retval <0  写失败
XM_S32 LibXmMaQue_Misc_setLedState(XM_S32 red, XM_S32 green, XM_S32 blue);

/// 获取Reset键状态。
/// 
/// \param [out] pValue 获取reset按键状态 1 : reset按键没按		0 : reset按下。
/// \retval 0  写成功
/// \retval <0  写失败

XM_S32 LibXmMaQue_Misc_readResetKey(XM_U32 *pValue);


/*
【功能描述】
	系统运行指示灯操作。
【参数】
	  参数名称				描述
	bOpen [in]		是否打开系统运行指示灯，1:点亮；0:关闭。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	如果板子没有系统运行指示灯，不要调用该API。
*/
XM_S32 LibXmMaQue_Misc_openSystemLed(XM_S32 bOpen);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_MISC_H

