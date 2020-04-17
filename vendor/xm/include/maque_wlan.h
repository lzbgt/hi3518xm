
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_wlan.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.04.12
 * 功能描述: 声明无线操作相关对外接口。
 *
 * 修改历史:  
 ** 1.日期: 2019.04.12
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.04.12
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#ifndef MAQUE_WLAN_H
#define MAQUE_WLAN_H


#ifdef __cplusplus
extern "C" {
#endif

/// 注意::: 本文件提供的接口需要配合后台配网业务一起实现，进程间通信主要涉及3个文件: 
/// 1.  notifyToChangeWIFIMODEFIFO : wifi 状态切换交互文件，如AP -->STA，STA-->AP。通信值参考: MaQueWLanSwitchState_e
/// 2.  WIFIConfigInfo : ap 热点信息描述，参考: MaQueConfigWlan_s
/// 3.  stareadyfifo : station 模式就绪通知文件，参考: MaQueWpaState_e

// 以下文件路径由LibXmMaQue_System_startUp 中的stStartParam.aWritableDir 指定
#define FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO "notifyToChangeWIFIMODEFIFO"
#define FILE_WIFI_CONFIG_INFO "WIFIConfigInfo"
#define FILE_STA_READY_FIFO "stareadyfifo"

#define IW_ENCODING_TOKEN_MAX 128

//该枚举 从wpa_supplicant 工具中拷贝过来.
typedef enum
{
	WPA_DISCONNECTED,
	WPA_INTERFACE_DISABLED,
	WPA_INACTIVE,
	WPA_SCANNING,
	WPA_AUTHENTICATING,
	WPA_ASSOCIATING,
	WPA_ASSOCIATED,
	WPA_4WAY_HANDSHAKE,
	WPA_GROUP_HANDSHAKE,
	WPA_COMPLETED
} MaQueWpaState_e;

typedef enum
{
	MAQUE_SWITCH_NONE,
	MAQUE_SWITCH_TO_AP,
	MAQUE_SWITCH_TO_STA,
	MAQUE_SWITCH_TO_SEARCH,
	MAQUE_SWITCH_NR
} MaQueWLanSwitchState_e;

typedef enum
{
	MAQUE_WPA_NOT_EXIST = -1,
	MAQUE_WPA_UNLINK = 0,
	MAQUE_WPA_LINK = 1,
	MAQUE_WPA_NR
} MaQueWpaLinkState_e;

typedef enum
{
	MAQUE_ENCRY_NONE,
	MAQUE_ENCRY_WEP_OPEN,
	MAQUE_ENCRY_AES,
	MAQUE_ENCRY_TKIP,
	MAQUE_ENCRY_NR
} MaQueEncrypType_e;

typedef enum
{
	MAQUE_KEY_HEX,
	MAQUE_KEY_ASCII,
	MAQUE_KEY_NR
} MaQueKeyType_e;

typedef struct 
{
	XM_CHAR aSsid[32];
	XM_S32 auth;
	MaQueEncrypType_e eEncryp;
	MaQueKeyType_e keyType;  	//0:Hex 1:ASCII
	XM_CHAR aPasswd[IW_ENCODING_TOKEN_MAX];
	XM_CHAR aMac[8];
	XM_S32 chn;
	XM_S32 rssi;
	XM_S32 aReserved[3];
} MaQueConfigWlan_s;

typedef struct
{
    XM_S32 count;  //搜寻到的wifi网络数量
    MaQueConfigWlan_s aNet[128];//每个wifi网络的相关信息
} MaQueWlanSearchInfo_s;

/*
【功能描述】
	搜索wifi 热点。注意: 该接口主要先通知后台配网进程发起搜索功能并且将搜索结果写到/var/tmp/iwlistinfo 文件，
	之后由本接口解析。往文件FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO 写入枚举MAQUE_SWITCH_TO_SEARCH 即为通知配网进程
	发起搜索功能，配网进程需要做相应处理。
【参数】
	  参数名称			描述
	pstWlanSearchInfo [out]	wifi 热点描述，参考MaQueWlanSearchInfo_s。
	len [out]		暂时无效。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Wlan_searchWIFI(MaQueWlanSearchInfo_s *pstWlanSearchInfo, XM_S32 len);

/*
【功能描述】
	获取AP 连接状态。注意: 配网后台进程需要将wpa 的配网状态(MaQueWpaState_e) 写入文件: /var/tmp/wpa_state 。
【参数】
	  参数名称			描述
	pEthName [in]	网络接口名，默认eth2 。
【返回值】
	AP 连接状态，参考MaQueWpaLinkState_e 。
*/
MaQueWpaLinkState_e LibXmMaQue_Wlan_getWpaStatus(XM_CHAR *pEthName);

/*
【功能描述】
	wifi 切换到AP 模式。注意: 该接口主要先把AP 热点描述信息写入文件FILE_WIFI_CONFIG_INFO，再通过管
	道FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO 写入枚举MAQUE_SWITCH_TO_AP 通知配网进程做相应处理。
【参数】
	  参数名称			描述
	pstWlan [in]	wifi 热点描述，参考MaQueConfigWlan_s。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Wlan_switch2AP(MaQueConfigWlan_s *pstWlan);

/*
【功能描述】
	wifi 切换到STA 模式。注意: 该接口主要先把AP 热点描述信息写入文件FILE_WIFI_CONFIG_INFO，再通过管
	道FILE_NOTIFY_TO_CHANGE_WIFI_MODE_FIFO 写入枚举MAQUE_SWITCH_TO_STA 通知配网进程做相应处理。
【参数】
	  参数名称			描述
	pstWlan [in]	wifi 热点描述，参考MaQueConfigWlan_s。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
*/
XM_S32 LibXmMaQue_Wlan_switch2STA(MaQueConfigWlan_s *pstWlan);

#ifdef __cplusplus
}
#endif

#endif //MAQUE_TIME_H

