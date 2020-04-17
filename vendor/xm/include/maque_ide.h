
#ifndef MAQUE_IDE_H
#define MAQUE_IDE_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{ 
	FS_TYPE_UNKNOWN,
	FS_TYPE_FAT32,
	FS_TYPE_EXFAT,
	FS_TYPE_MAX
} MaQueFsType_e;

typedef struct
{
	MaQueFsType_e eFsType;
	XM_CHAR* pDev;

	//以下参数暂时只对exfat有效
    XM_S32 spc; 	//每个簇多少个扇区，默认64
    XM_CHAR *pVolumeLabel;
    XM_UL volumeSerial;
    XM_U64 firstSector;
} MaQueMakeFs_s;

/*
【功能描述】
	雄迈私有wfs 系统格式化，目前暂时无效。
【参数】
	pDev : 设备节点。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	无。
*/
XM_S32 LibXmMaQue_Ide_format(const XM_CHAR *pDev);

/*
【功能描述】
	FAT32 exFAT 格式化。
【参数】
	pstMkFs , 参考MaQueMakeFs_s 。
【返回值】
	XM_SUCCESS		成功。
	其它			失败，请参考错误码。
【注意】
	无。
*/
XM_S32 LibXmMaQue_Ide_makeFs(MaQueMakeFs_s *pstMkFs);

#ifdef __cplusplus
}
#endif


#endif //MAQUE_IDE_H

