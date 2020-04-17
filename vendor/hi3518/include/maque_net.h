
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_net.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 声明网络操作相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_NET_H
#define MAQUE_NET_H


#ifdef __cplusplus
extern "C" {
#endif

/*
【功能描述】
	设置板子的IP地址和子网掩码。
【参数】
	  参数名称			描述	
	pEthName [in]	网络接口名。
	pHostIp	[in]	需设置的IP地址。形似：192.168.1.100	
	pNetMask [in]	需设置的子网掩码。形似：255.255.255.0
【返回值】
	XM_SUCCESS	成功。
	其它	失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Net_setHostIP(XM_CHAR *pEthName, XM_CHAR *pHostIp, XM_CHAR *pNetMask, XM_CHAR *pGateway);

/*
【功能描述】
	获取板子的IP地址和子网掩码。
【参数】
	  参数名称			描述
	pEthName [in]	网络接口名。
	pHostIp	[out]	指向获取的IP地址。形似：192.168.1.100
	hostIpLen [in]	给pHostIp申请的Buffer大小。注意：应不少于16个字节。
	pNetMask [out]	指向获取的子网掩码。形似：255.255.255.0
	netMaskLen [in]	给pNetMask申请的Buffer大小。注意：应不少于16个字节。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Net_getHostIP(XM_CHAR *pEthName, XM_CHAR *pHostIp, XM_S32 hostIpLen, 
			XM_CHAR *pNetMask, XM_S32 netMaskLen, XM_CHAR *pGateway, XM_S32 gatewayLen);

#if 0
/*
【功能描述】
	设置板子的网关。
【参数】
	  参数名称			描述
	pEthName [in]	网络接口名。
	pGateway [in]	需设置的网关。形似：192.168.1.1
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Net_setGateway(XM_CHAR *pEthName, XM_CHAR *pGateway);

/*
【功能描述】
	获取板子的网关。
【参数】
	  参数名称			描述
	pEthName [in]	网络接口名。
	pGateway [out]	指向获取的网关。形似：192.168.1.1
	gatewayLen [in]		给pGateway申请的Buffer大小。注意：应不少于16个字节。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Net_getGateway(XM_CHAR *pEthName, XM_CHAR *pGateway, XM_S32 gatewayLen);
#endif

/*
【功能描述】
	获取设置到网卡驱动的Mac。
【参数】
	  参数名称			描述
	pEthName [in]	网络接口名。
	pMac [out]		指向获取的Mac。形似：00:01:23:34:56:78
	macLen [in]		给pMac申请的Buffer大小。注意：应不少于18个字节。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Net_getMac(XM_CHAR *pEthName, XM_CHAR *pMac, XM_S32 macLen);


#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

