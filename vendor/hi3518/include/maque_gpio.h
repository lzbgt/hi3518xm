
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_gpio.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.05.15
 * 功能描述: 声明GPIO相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_GPIO_H
#define MAQUE_GPIO_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	XM_U8 group;
	XM_U8 num;
	XM_U8 value;
} GpioParam_s;


/*
【功能描述】
	设置GPIO高。
【参数】
	  参数名称			描述
	gpioGroup [in]	GPIO组号。
	gpioNum [in]	GPIO序号。
	pOwnerName [in]	调用者标识名。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Gpio_setHigh(XM_U32 gpioGroup, XM_U32 gpioNum, XM_CHAR *pOwnerName);

/*
【功能描述】
	设置GPIO低。
【参数】
	  参数名称			描述
	gpioGroup [in]	GPIO组号。
	gpioNum [in]	GPIO序号。
	pOwnerName [in]	调用者标识名。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Gpio_setLow(XM_U32 gpioGroup, XM_U32 gpioNum, XM_CHAR *pOwnerName);

/*
【功能描述】
	读取GPIO值。
【参数】
	  参数名称			描述
	gpioGroup [in]	GPIO组号。
	gpioNum [in]	GPIO序号。
	pValue [out]	保存读取的GPIO值。
	pOwnerName [in]	调用者标识名。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Gpio_read(XM_U32 gpioGroup, XM_U32 gpioNum, XM_U32 *pValue, XM_CHAR *pOwnerName);


#ifdef __cplusplus
}
#endif

#endif

