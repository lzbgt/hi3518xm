
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_env.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明Env分区读写相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_ENV_H
#define MAQUE_ENV_H


#ifdef __cplusplus
extern "C" {
#endif

/*
【功能描述】
	设置Env分区的键值。为了提交效率，系统启动时，会把Env分区读到内存中作为备份。读操作，
	就从内存备份区获取；写操作，先把键值对设置到内存备份区，计算CRC32校验后，再写到Flash
	中。如果Env分区已有相同的键，将会先删除原有的，再在后面添加新的。
	本API只是把键值对设置到内存的备份区中，并没有实际写到Flash中。写到Flash的操作，由
	下面API：LibXmMaQue_Env_save()完成。与下面API：LibXmMaQue_Env_writeKeyValue()相比，
	后者相当于集成了LibXmMaQue_Env_setKeyValue()和LibXmMaQue_Env_save()功能，即不但会
	修改内存备份区，同时还会写入Flash。注意这三个API的区别。提供本API的好处是，当有多
	个键值对需要更新时，先更新到内存备份区，最后一次性写入Flash，可大大提高效率，同时
	增加Flash的使用寿命。
【参数】
	  参数名称			描述
	pKey  [in]		需要设置的键名。
	pValue [in]		需要设置的键值。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	调用LibXmMaQue_Env_setKeyValue()，只把键值对设置到内存备份区，不会写到Flash Env
	分区，调用下面介绍的LibXmMaQue_Env_getValue()可以获取到，但断电后会丢失。要想保
	存到Flash中，可以调用LibXmMaQue_Env_save()。或直接调用LibXmMaQue_Env_writeKeyValue()。
*/
XM_S32 LibXmMaQue_Env_setKeyValue(XM_CHAR *pKey, XM_CHAR *pValue);

/*
【功能描述】
	把Env内存备份区的数据保存到Flash中。一般和上一个API：LibXmMaQue_Env_setKeyValue()配合使用。
【参数】
	无。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Env_save(XM_VOID);

/*
【功能描述】
	把函数传入的键值对，更新到Env内存备份区，然后保存到Flash中。如果Env分区已经有相同的键，
	将会先删除原有的，再在后面添加新的。注意：和前面API：LibXmMaQue_Env_setKeyValue()的区
	别。相当于集成了LibXmMaQue_Env_setKeyValue()和LibXmMaQue_Env_save()功能。
【参数】
	  参数名称			描述	
	pKey [in]		需要保存到Flash中的键名。
	pValue [in]		需要保存到Flash中的键值。
【返回值】
	XM_SUCCESS	成功。
	其它	失败，请参考错误码。
【注意】
	调用LibXmMaQue_Env_writeKeyValue()，把键值对设置到内存备份区，同时写到Flash Env分区，
	频繁调用会影响Flash寿命。
*/
XM_S32 LibXmMaQue_Env_writeKeyValue(XM_CHAR *pKey, XM_CHAR *pValue);

/*
【功能描述】
	读取Env分区pKey对应的值。如果Env分区找不到pKey对应的键值，将返回失败。
【参数】
	  参数名称			描述
	pKey [out]		需要读取其值的键名。
	pValue [out]	保存所获取键值的Buffer指针。
	valueLen [in]	分配给pValue的最大空间（单位：字节），包括字符串的结束符。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
给pValue分配的Buffer要足够大，否则获取的字符串被截断。
*/
XM_S32 LibXmMaQue_Env_getValue(XM_CHAR *pKey, XM_CHAR *pValue, XM_S32 valueLen);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_ENV_H

