
#ifndef __MAQUE_MTDVER__
#define __MAQUE_MTDVER__

/*
【功能描述】
	获取uboot、kernel、rootfs 版本。
【参数】
	  参数名称			描述
	pDate [out]	依次是uboot、kernel、rootfs 版本。
	pNum [out]  输出参数个数。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	该接口与系统制作相关，不通用。
*/
XM_S32 LibXmMaQue_Mtd_getMtdVersion(XM_U32 *pDate, XM_S32 *pNum);

/*
【功能描述】
	设置SN 序列号。
【参数】
	  参数名称			描述
	pBuf	[in]	SN 序列号。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	该接口与系统制作相关，不通用。
*/
XM_S32 LibXmMaQue_Mtd_setIMEI(XM_U8 *pBuf);

/*
【功能描述】
	获取SN 序列号。
【参数】
	  参数名称			描述
	pBuf	[in]	SN 序列号。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	该接口与系统制作相关，不通用。
*/
XM_S32 LibXmMaQue_Mtd_getIMEI(XM_U8 *pBuf);

#endif

