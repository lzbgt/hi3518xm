
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_rs485.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.05.15
 * 功能描述: 声明RS485读写相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_RS485_H
#define MAQUE_RS485_H


#ifdef __cplusplus
extern "C" {
#endif

/// 串口数据位数
typedef enum  
{
	MAQUE_UART_DATA_BITS_5 = 5,
	MAQUE_UART_DATA_BITS_6,
	MAQUE_UART_DATA_BITS_7,
	MAQUE_UART_DATA_BITS_8,
	MAQUE_UART_DATA_BITS_NR
} MaQueUartDataBits_e;

/// 串口校验位类型
typedef enum  
{
	MAQUE_UART_PARITY_NO,	 ///< 无校验
	MAQUE_UART_PARITY_ODD, 	 ///< 奇校验
	MAQUE_UART_PARITY_EVEN,	 ///< 偶校验
	MAQUE_UART_MARK,		 ///< 校验位始终为1
	MAQUE_UART_SPACE		 ///< 校验位始终为0
} MaQueUartParity_e;

/// 串口停止位类型
typedef enum 
{
	MAQUE_UART_STOPBITS_ONE,	///< 1 stop bit
	MAQUE_UART_STOPBITS_ONE5,	///< 1.5 stop bit
	MAQUE_UART_STOPBITS_TWO		///< 2 stop bit
} MaQueUartStopBits_e;


/// 串口属性结构
typedef struct
 {
	XM_U32 baudrate;	///< 实际的波特率值。		
	MaQueUartDataBits_e eDataBits;	///< 实际的数据位数，取UartDataBits_e类型的枚举值。	
	MaQueUartParity_e eParity;	///< 奇偶校验选项，取UartParity_e类型的枚举值。	
	MaQueUartStopBits_e eStopBits;	///< 停止位数，取UartStopBits_e类型的枚举值。	
	XM_U8 reserved;	///< 保留	
} MaQueUartAttr_s;


/*
【功能描述】
	创建RS485通信接口。
【参数】
	 无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_RS485_create(XM_VOID);

/*
【功能描述】
	设置RS485通道属性。
【参数】
	 参数名称			描述
	pstAttr [in]	需要设置的通道属性。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/

XM_S32 LibXmMaQue_RS485_setAttr(MaQueUartAttr_s *pstAttr);


/*
【功能描述】
	写数据到RS485总线上。
【参数】
	  参数名称			描述
	pData [in]		指向需要写入RS485总线的数据。
	dataLen [in]	需要写入RS485总线的数据长度。
【返回值】
	大于0			实际写入的数据长度。
	小于等于0		失败，请参考错误码。
*/
XM_S32 LibXmMaQue_RS485_write(XM_CHAR *pData, XM_U32 dataLen);

/*
【功能描述】
	从RS485总线上读取数据。
【参数】
	  参数名称			描述
	pData [out]		存放从RS485总线读取数据的缓存。
	dataLenMax [in]	从RS485总线读取数据的最大长度，一般为pData所指的缓存大小。
【返回值】
	大于0			实际读入的数据长度。
	小于等于0		失败，请参考错误码。
*/
XM_S32 LibXmMaQue_RS485_read(XM_CHAR *pData, XM_U32 dataLenMax);

/*
【功能描述】
	从RS485总线上读取数据，用户可以传入超时时间，总超时为: timeoutSec秒 + timeoutUs微秒。
【参数】
	  参数名称			描述
	pData [out]		存放从RS485总线读取数据的缓存。
	dataLenMax [in]	从RS485总线读取数据的最大长度，一般为pData所指的缓存大小。
	timeoutSec [in]	超时时间秒部分。
	timeoutUs [in]	超时时间微秒部分。
【返回值】
	大于0			实际读入的数据长度。
	小于等于0		失败，请参考错误码。
*/
XM_S32 LibXmMaQue_RS485_readTimeOut(XM_CHAR *pData, XM_U32 dataLenMax, XM_U32 timeoutSec, XM_U32 timeoutUs);

/*
【功能描述】
	销毁RS485通信接口。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_RS485_destroy(XM_VOID);


#ifdef __cplusplus
}
#endif

#endif

