
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_flash.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明Flash读写等操作相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_FLASH_H
#define MAQUE_FLASH_H


#ifdef __cplusplus
extern "C" {
#endif

/*
【功能描述】
	初始化Flash操作。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	无。
*/
XM_S32 LibXmMaQue_Flash_open(XM_VOID);

/*
【功能描述】
	从Flash偏移addr地址处，读取大小为size的数据，保存到pBuf中。

【参数】
	  参数名称			描述
	pBuf [out]		保存所读取数据的Buffer指针。
	addr [in]		需要读取的Flash偏移地址。
	size [in]		需要读取的数据大小。
【返回值】
	大于0		实际读取的数据大小。
	其它		失败，请参考错误码。
【注意】
	LibXmMaQue_Flash_open ()调用成功后，才可以使用本API。
*/
XM_S32 LibXmMaQue_Flash_read(XM_U8 *pBuf, XM_UL addr, XM_U32 size);

/*
【功能描述】
	把pBuf所指空间大小为size的数据，写入Flash偏移addr地址处。
【参数】
	  参数名称		描述
	pBuf [in]	写到Flash的数据缓存。
	addr [in]	往Flash该偏移地址写数据。
	size [in]	需要写入的数据大小。
【返回值】
	大于0	实际写入的数据大小。
	其它	失败，请参考错误码。
【注意】
	LibXmMaQue_Flash_open()调用成功后，才可以使用本API
	考虑到效率，实际没有按"读-修改-擦除-写"这样步骤写，而是直接擦除后写，因此如果
	不是整块写，需要调用者自己做好数据保护工作。Flash擦除块大小，可由下面介绍的API：
	LibXmDvr_Flash_getEraseSize()获取。
*/
XM_S32 LibXmMaQue_Flash_write(XM_U8 *pBuf, XM_UL addr, XM_U32 size);

/*
【功能描述】
	获取当前Flash支持的擦除块大小。
【参数】
	无。
【返回值】
	大于0	Flash支持的擦除块大小。一般Spi Nor Flash的擦除块大小为64KB。
	其它	失败，请参考错误码。
【注意】
	考虑到效率，LibXmDvr_Flash_write()实际没有按"读-修改-擦除-写"这样步骤写，而是
	直接擦除后写，因此如果不是整块写，需要调用者自己做好数据保护工作，这时就需要
	获取Flash支持的擦除块大小。
*/
XM_S32 LibXmMaQue_Flash_getEraseSize(XM_VOID);

/*
【功能描述】
	获取当前Flash的ID。
【参数】
	无。
【返回值】
	-1		失败。
	其它	Flash ID。例如：
  				华邦8MB：0xEF4017
  				华邦16MB：0xEF4018
  				旺宏8MB：0xC22017
  				旺宏16MB：0xC22018
*/
XM_S32 LibXmMaQue_Flash_getId(XM_VOID);

/*
【功能描述】
	获取当前Flash的名称。
【参数】
	无。
【返回值】
	NULL	失败。
	其它	Flash 名称。例如：
 				w25q64/s25fl064k
  				w25q128
【注意】
	它是内核Flash驱动返回的。每个平台可能有差异，只用于调试。
*/
XM_CHAR *LibXmMaQue_Flash_getName(XM_VOID);

/*
【功能描述】
	关闭Flash操作。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Flash_close(XM_VOID);

/*
【功能描述】
	分区升级预处理。
【参数】
	  参数名称		描述
	addr [in]	分区开始地址。
	size [in]	分区大小。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	该接口只适用支持备份系统的设备。
*/
XM_S32 LibXmMaQue_Flash_prePart(XM_UL addr, XM_U32 size);

/*
【功能描述】
	分区升级后处理。
【参数】
	  参数名称		描述
	addr [in]	分区开始地址。
	size [in]	分区大小。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	该接口只适用支持备份系统的设备。
*/
XM_S32 LibXmMaQue_Flash_postPart(XM_UL addr, XM_U32 size);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_FLASH_H

