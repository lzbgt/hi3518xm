
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_crc.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.05.15
 * 功能描述: 声明CRC计算相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.05.15
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_CRC_H
#define MAQUE_CRC_H


#ifdef __cplusplus
extern "C" {
#endif


/*
【功能描述】
	对pDat所指的长度为Len的数据进行32位Crc校验计算，最后返回计算结果。
【参数】
	  参数名称		描述
	crc [in]	是初始值或者前一次Crc32计算所得的值，首次调用一般传0。
	pDat [in]	需要计算Crc的数据指针。
	len [in]	需要计算Crc的数据长度。
【返回值】
	由32位Crc计算所得的值。
【注意】
	把一块n字节的数据传给LibXmMaQue_Crc_crc32()（设置参数crc为0）进行计算。
	和把这块数据分成多份（大小随意），分别传给LibXmMaQue_Crc_crc32()（设置
	参数crc首次为0，后续为上次计算结果）进行计算，最后的结果是相同的。
*/
XM_U32 LibXmMaQue_Crc_crc32(XM_U32 crc, const XM_VOID *pDat, XM_S32 len);


#ifdef __cplusplus
}
#endif

#endif

