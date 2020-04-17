
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_string.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ������ʾ�����õ����ַ���������ؽӿں�����
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_STRING_H
#define MAQUE_DEMO_STRING_H

typedef struct 
{
	XM_U32 enumValue;  /* ö��ֵ��*/
	XM_CHAR *pStr;  /* ö��ֵ��Ӧ�����ơ�*/
} EnumStr_s;

/*
���������pStr����Ҫת����ö�٣�Enum�����ַ�����
	pstStrEnum����������֧�ֵ�ö��ֵ�Ͷ�Ӧֵ�������顣�ο���">�ؼ��ṹ��>>EnumStr_s"��
	nEntries��pstStrEnum�а����Ľṹ��������
����������ޡ�
���أ��ɹ���ö��ֵ��ʧ�ܣ�nEntries��
�������ܣ���pstStrEnum�����飩�в����봫�����pStrƥ���ö��ֵ��������ҵ����ͷ��ظ�ö��ֵ��
	���򷵻�nEntries���������ǲ�����Чö��ֵ��Χ�ڣ���
*/
XM_S32 MaQue_Demo_String_toEnum(XM_CHAR *pStr, EnumStr_s *pstStrEnum, XM_S32 nEntries);

/*
���������enumValue����Ҫת����ֵ����ö�٣�Enum��ֵ��
	 pstStrEnum����������֧�ֵ�ö��ֵ�Ͷ�Ӧֵ�������顣�ο���">�ؼ��ṹ��>>EnumStr_s"��
	 nEntries��pstStrEnum�а����Ľṹ��������
����������ޡ�
���أ��ɹ���ö��ֵ�����ַ�������ʧ�ܣ�"NULL"��
�������ܣ���pstStrEnum�����飩�в����봫�����enumValueƥ���ö��ֵ������ҵ���
	�ͷ��ظ�ö��ֵ�������򷵻�"NULL"��
*/
XM_CHAR *MaQue_Demo_String_enum2Str(XM_U32 enumValue, EnumStr_s *pstStrEnum, XM_S32 nEntries);


XM_CHAR *MaQue_Demo_String_trimLeftSpace(XM_CHAR *pStr);
XM_CHAR *MaQue_Demo_String_trimRightSpace(XM_CHAR *pStr);
XM_CHAR *MaQue_Demo_String_trimLeftRightSpace(XM_CHAR *pStr);

#endif

