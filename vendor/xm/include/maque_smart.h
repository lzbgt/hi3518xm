
#ifndef MAQUE_SMART_H
#define MAQUE_SMART_H

#include "maque_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAQUE_MAX_CLASS_NUM 2
#define MAQUE_MAX_RECT_NUM 10

typedef enum
{
	MAQUE_SMART_ROI_MB_MODE_4X4,
	MAQUE_SMART_ROI_MB_MODE_8X8,
	MAQUE_SMART_ROI_MB_MODE_16X16,
	MAQUE_SMART_ROI_MB_MODE_BUTT
} MaQueSmartRoiMbMode_e;

typedef struct
{
	XM_S32 		bEnabled; 		/* ���Ρ�����ʹ�ܿ��� 0 : �رգ�1 : ����*/
	XM_FLOAT 	pdThreshold; 	/* ��ֵ����Χ[0.0,1.0)��ֵԽ��SMD �澯�����μ��Խ�������� */
	XM_FLOAT 	fdThreshold;	/* ��ֵ����Χ[0.0,1.0)��ֵԽ���������Խ�������� */
	XM_S32 		bRoiEnabled;	/* ����Ȥ������ڿ��� */
	XM_U32		roiThreshold; 	/* ��ֵ��ȡֵ��Χ[1,1024]�� ֵԽ�󣬸���Ȥ����� SMD �澯�����μ����������Խ�������� */
	MaQueSmartRoiMbMode_e roiMbMode; 	/* ����Ȥ������ģʽ*/
	XM_U32 		imgWidth; 		/* ����Ȥ������ͼ��Ŀ��̶�640 */
	XM_U32 		imgHeight; 		/* ����Ȥ������ͼ��ĸ� ���̶�360*/
	XM_U8 		*pMbMap; 		/* ���ӳ���.for 4x4 mb mode, alloc (DIV_UP(img_width,4))*(DIV_UP(img_height,4)) bytes */
} MaQueSmartParams_s;

typedef struct
{
    XM_S16 s16X1;        /**< ���Ͻ�x���� */
    XM_S16 s16Y1;        /**< ���Ͻ�y���� */
    XM_S16 s16X2;        /**< ���½�x���� */
    XM_S16 s16Y2;        /**< ���½�y���� */
} MaQueSmartRect_s;

typedef struct
{
	XM_S32 targetPDNum; 					//����Ŀ�����
	XM_S32 targetFDNum; 					//����Ŀ�����
	MaQueSmartRect_s aPDRect[MAQUE_MAX_RECT_NUM];	  	//����Ŀ������
	MaQueSmartRect_s aFDRect[MAQUE_MAX_RECT_NUM];	  	//����Ŀ������
	XM_S32 aReserved[8];
} MaQueSmartTarget_s;

typedef struct
{
    XM_S32 bLowBitrate; 	/* ����PQ ������ʹ�ܿ���*/
	XM_S32 bIsoAdaptive; 	/* ���� ISO ��������������Ӧ���ڿ���*/
	XM_S32 aReserved[6];
} MaQueSmartLowBitRate_s;

typedef struct
{
	XM_U32 nWidth;    /* ��ץȡͼƬ�Ŀ�*/
	XM_U32 nHeight;   /* ��ץȡͼƬ�Ŀ�*/
	XM_U8 *pBuffer;
	XM_S32 nBufLen;   /* ���nBufLen > 0�����ɵ����߸�pBuffer�����ڴ棻nBufLen<= 0��
						�ɵײ�����ڴ棬�����߸����ͷš�Ŀǰ��֧�ֵײ�����ڴ�ķ�ʽ��*/
	XM_S32 nDataLen;  /* ����ʵ��Jpeg������С��*/
	XM_U64 pts;	
	XM_HANDLE handleMem;
	XM_S32 nToltalJpeg;	/* Сͼ������*/
	XM_S32 nIndex;		/* Сͼ��������0��ʼ��*/
	XM_CHAR aClassName[4]; /* ����: pd������: fd ����ͼ : pic */
	XM_U8 aReserved[4]; /* ������*/
} MaQueSmartJpegFrame_s;

typedef XM_S32 (*MaQueSmartGetJpegCallbackFuncPtr) (XM_VOID *pUserArg, MaQueSmartJpegFrame_s *pstJpegFrame);

typedef struct 
{
	MaQueSmartGetJpegCallbackFuncPtr pCallbackFuncPtr;
	XM_VOID *pCallbackArg;	
} MaQueSmartJpegCallback_s;

typedef enum
{
	MAQUE_SMART_JPEG_ENCODE_LARGE,		/// ��֡ͼƬ��
	MAQUE_SMART_JPEG_ENCODE_CUTOUT,		/// ��֡ͼƬ�е�������������Сͼ��
	MAQUE_SMART_JPEG_ENCODE_NR,
} MaQueSmartJpegEncodeType_e;

typedef enum
{
	MAQUE_SMART_JPEG_CLASS_NONE = 0x0,
	MAQUE_SMART_JPEG_CLASS_PD = 0x1,
	MAQUE_SMART_JPEG_CLASS_FD = 0x2,
	MAQUE_SMART_JPEG_CLASS_NR,
} MaQueSmartJpegClass_e;

typedef struct
{
	MaQueSmartJpegCallback_s stCallback;
	MaQueSmartJpegEncodeType_e eType;
	MaQueSmartJpegClass_e eClass; // Сͼ���ͣ���ͼ�Ĳ�����Ч�����κ�����Сͼͬʱ����: MAQUE_SMART_JPEG_CLASS_PD|MAQUE_SMART_JPEG_CLASS_FD
	XM_S32 aReserved[7]; /* ������*/
} MaQueSmartJpegCallbackParam_s;

/*
������������
	�������������ι��ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartCreate(XM_S32 channel);

/*
������������
	��ȡ����������Ŀ�������������Ϣ��������Ϣ�����(640 * 360 ) ��������ϵ��
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstMaQueSmartTarget [out]  ���������ο�: MaQueSmartTarget_s��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartGetTarget(XM_S32 channel, MaQueSmartTarget_s *pstMaQueSmartTarget);

/*
������������
	�������������μ�������
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	pstMaQueSmartParams [in] ���ò������ο�: pstMaQueSmartParams ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartSetParameter(XM_S32 channel, MaQueSmartParams_s *pstMaQueSmartParams);

/*
������������
	�������������ι��ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartDestory(XM_S32 channel);

/*
������������
	���õ����ʹ��ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	eStreamChn [in] ��С������֧�ָ������������ã�ֵ�ο�: MaQueStreamChannel_e ��
	pstMaQueSmartLowBitRate [in] ���������ò������ο�: MaQueSmartLowBitRate_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartLowBitRate(XM_S32 channel, MaQueStreamChannel_e eStreamChn, MaQueSmartLowBitRate_s *pstMaQueSmartLowBitRate);

/*
������������
	��������PQ ���ܡ�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	bEnable [in]	0 : ʧ�ܣ�1 : ʹ�ܡ�
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartAdvanceIsp(XM_S32 channel, XM_S32 bEnable);

/*
������������
	���μ����ֵ���á���LibXmMaQue_SmartSetParameter �е�pdThreshold ��һ��Ч�����˴��ɵ�����̬���á�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	threshold [in]	���μ����ֵ��Ĭ��0.75���Ƽ����ڷ�Χ0.65 ~ 0.85 ֮�䡣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartPDThreshold(XM_S32 channel, XM_FLOAT threshold);

/*
������������
	���������ֵ���á���LibXmMaQue_SmartSetParameter �е�fdThreshold ��һ��Ч�����˴��ɵ�����̬���á�
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	threshold [in]	���������ֵ��Ĭ��0.63���Ƽ����ڷ�Χ0.6 ~ 0.8 ֮�䡣
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartFDThreshold(XM_S32 channel, XM_FLOAT threshold);

/*
������������
	ע������ץͼ���ݻص���һ��ͼƬ����һ�λص���
��������
	  ��������			����
	channel [in]	��Ӧsensor����ͨ������0��ʼ������豸ֻ��һ��sensor��ֵӦ����Ϊ0��
	MaQueSmartFdJpegCallbackParam_s [in]	�ص��������ο�MaQueSmartFdJpegCallbackParam_s ��
������ֵ��
	XM_SUCCESS		�ɹ���
	����			ʧ�ܣ���ο������롣
*/
XM_S32 LibXmMaQue_SmartRegisterCallback(XM_S32 channel, MaQueSmartJpegCallbackParam_s *pstCallback);

#ifdef __cplusplus
}
#endif


#endif //MAQUE_IDE_H

