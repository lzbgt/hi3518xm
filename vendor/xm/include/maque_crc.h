
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_crc.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.05.15
 * ��������: ����CRC������ض���ӿڡ�
 *
 * �޸���ʷ:  
 ** 1.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.05.15
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/
 

#ifndef MAQUE_CRC_H
#define MAQUE_CRC_H


#ifdef __cplusplus
extern "C" {
#endif


/*
������������
	��pDat��ָ�ĳ���ΪLen�����ݽ���32λCrcУ����㣬��󷵻ؼ�������
��������
	  ��������		����
	crc [in]	�ǳ�ʼֵ����ǰһ��Crc32�������õ�ֵ���״ε���һ�㴫0��
	pDat [in]	��Ҫ����Crc������ָ�롣
	len [in]	��Ҫ����Crc�����ݳ��ȡ�
������ֵ��
	��32λCrc�������õ�ֵ��
��ע�⡿
	��һ��n�ֽڵ����ݴ���LibXmMaQue_Crc_crc32()�����ò���crcΪ0�����м��㡣
	�Ͱ�������ݷֳɶ�ݣ���С���⣩���ֱ𴫸�LibXmMaQue_Crc_crc32()������
	����crc�״�Ϊ0������Ϊ�ϴμ����������м��㣬���Ľ������ͬ�ġ�
*/
XM_U32 LibXmMaQue_Crc_crc32(XM_U32 crc, const XM_VOID *pDat, XM_S32 len);


#ifdef __cplusplus
}
#endif

#endif

