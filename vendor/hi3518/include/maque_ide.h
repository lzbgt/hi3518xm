
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

	//���²�����ʱֻ��exfat��Ч
    XM_S32 spc; 	//ÿ���ض��ٸ�������Ĭ��64
    XM_CHAR *pVolumeLabel;
    XM_UL volumeSerial;
    XM_U64 firstSector;
} MaQueMakeFs_s;

/*
������������
	����˽��wfs ϵͳ��ʽ����Ŀǰ��ʱ��Ч��
��������
	pDev : �豸�ڵ㡣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ޡ�
*/
XM_S32 LibXmMaQue_Ide_format(const XM_CHAR *pDev);

/*
������������
	FAT32 exFAT ��ʽ����
��������
	pstMkFs , �ο�MaQueMakeFs_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
��ע�⡿
	�ޡ�
*/
XM_S32 LibXmMaQue_Ide_makeFs(MaQueMakeFs_s *pstMkFs);

#ifdef __cplusplus
}
#endif


#endif //MAQUE_IDE_H

