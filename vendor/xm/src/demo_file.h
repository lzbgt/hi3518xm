/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: demo_file.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.03.01
 * ��������: ������ʾ�����õ����ļ������Ͷ�д�ӿں�����
 *
 * �޸���ʷ:  
 ** 1.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.03.01
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEMO_FILE_H
#define MAQUE_DEMO_FILE_H


typedef enum 
{
	XM_VALUE_TYPE_INT,  /* ���� */
	XM_VALUE_TYPE_HEX,  /* 16������ */
	XM_VALUE_TYPE_STRING, /* �ַ��� */
	XM_VALUE_TYPE_NR
} XmValueType_e;

/*
����ṹ�����ڴ��ļ��л�ȡ�ַ���pString ��Ӧ������ΪeValTypeָ������󳤶�Ϊ
valSizeMax��ֵ��XM_VALUE_TYPE_INT��XM_VALUE_TYPE_HEX���͵��������ļ��б�������ַ�
������ȡ��浽pValue�е���������ֵ�������÷��ο���LibXmComm_File_parse512 ()��
*/
typedef struct
{
	XM_CHAR *pString;  /*�����߸�ֵ��ָ������ļ��л�ȡֵ���ַ�����*/
	XM_VOID *pValue;   /*�����߷���ռ䣬�����ļ��л�ȡ���ַ���pString��Ӧ��ֵ���������С�*/
	XmValueType_e eValType;  /*���ļ��л�ȡ���ַ�����Ӧ��ֵ���͡�INT��HEX�����ַ������ת��
							  *����ֵ�����浽pValue�С��䶨�壬�ο���xmcomm_struct.h��
							  */
	XM_U32 valSizeMax;	/*�����߸�pValue����Ŀռ��С��*/
} StringValueGet_s;

typedef XM_S32 (*DirEntComp_FuncPtr)(const struct dirent **, const struct dirent **);

typedef enum
{
	DIR_ENTRY_TYPE_NONE,
	DIR_ENTRY_TYPE_FILE,
	DIR_ENTRY_TYPE_DIR,
	DIR_ENTRY_TYPE_NR
} DirEntryType_e;


XM_S32 MaQue_Demo_File_Line_parse(XM_CHAR *pLine, StringValueGet_s *pstStrValGet, XM_S32 nEntriesMax);
XM_S32 MaQue_Demo_File_parse512(XM_CHAR *pFilePath, XM_CHAR *pModuleName,
			StringValueGet_s *pstStrValGet, XM_U32 nEntries);

XM_S64 MaQue_Demo_File_getSize(XM_CHAR *pFilePath);
XM_S32 MaQue_Demo_File_write(XM_S32 fd, XM_VOID *pBuf, XM_S32 count);
XM_S32 MaQue_Demo_File_read(XM_S32 fd, XM_VOID *pBuf, XM_S32 count);
XM_S32 MaQue_Demo_File_writeSimply(XM_CHAR *pFilePath, XM_U8 *pBuf, XM_S32 count);
XM_S32 MaQue_Demo_File_readSimply(XM_CHAR *pFilePath, XM_U8 *pBuf, XM_S32 count);

/*
���������pDirName��Ŀ¼·��������������ԣ�Ҳ�����Ǿ���·����
	eDirEntryType����Ҫ��ȡ��Ŀ¼�����ͣ�ָ��ΪDIR_ENTRY_TYPE_NONE��DIR_ENTRY_TYPE_NR��������ȡ�����ļ���Ŀ¼��
	pDirEntCompFuncPtr: ָ��Ŀ¼���ļ��������㷨������: alphasort��versionsort�ȡ�ΪNULLʱ��Ĭ��ʹ��versionsort��
				ע��alphasort����ĸ������˳�����У�versionsort����ĸ�����������С���С�����: a1001.txt, a20.txt
					�����ļ�������alphasort���򷽷���a1001.txt��ǰ����versionsort������a20.txt��ǰ��
					�û�Ҳ�����Լ����������㷨��Ҫ��:a > b������������ a == b������0��a < b�����ظ�����
���������pppstDirEntList�������ȡ���ļ���Ŀ¼�����ɺ����ڲ������ڴ棬���óɹ���ʹ�ú󣬵����߸����ͷ��ڴ档
					ע��: ����3άָ�롣
���أ��ɹ���XM_SUCCESS��ʧ�ܣ�XM_FAILURE��
�������ܣ�����C�⺯��scandir()����ȡpDirNameĿ¼�µ��ļ���Ŀ¼���ɹ��󣬸�pppstDirEntList�����ڴ棬���ѻ�ȡ��
	Ŀ¼�����pppstDirEntList[i]�С�����󣬵����߸����ͷ�pppstDirEntList��ָ���ڴ档
	ע��: pppstDirEntList��3άָ�롣��������Ҫ���ͷ�pppstDirEntList[i]�ڴ棬���ͷ�pppstDirEntList������
	   XM_S32 main(XM_VOID)
       {
           struct dirent **ppstDirEntList;
           XM_S32 n;

           n = LibXmComm_Dir_getDirOrFileName(".", DIR_ENTRY_TYPE_NONE, &ppstDirEntList);
           if (n < 0)
           {
           		ERROR_PRINTF("LibXmComm_Dir_getDirOrFileName() Failed!\n");
           }
           else 
           {
               while (n--) 
               {
                   INFO_PRINTF("%s\n", ppstDirEntList[n]->d_name);
                   free(ppstDirEntList[n]);
               }
               free(ppstDirEntList);
           }

           return 0;
       }
	Ҳ���Դ���ppstDirEntList�󣬵��ú���MaQue_Demo_Dir_freeDirEntryList()һ�����ͷš�
*/
XM_S32 MaQue_Demo_Dir_getDirOrFileName(XM_CHAR *pDirName, DirEntryType_e eDirEntryType, 
	struct dirent ***pppstDirEntList, DirEntComp_FuncPtr pDirEntCompFuncPtr);

/*
���������ppstDirEntList����Ҫ�ͷŵ�Ŀ¼���б���MaQue_Demo_Dir_getDirOrFileName()��á�
			nDirEntries����Ҫ�ͷŵ�Ŀ¼��������
����������ޡ�
���أ��ɹ���XM_SUCCESS��ʧ�ܣ�XM_FAILURE��
�������ܣ�����ͷ�ppstDirEntList[i],����ͷ�ppstDirEntList����
*/
XM_S32 MaQue_Demo_Dir_freeDirEntryList(struct dirent **ppstDirEntList, XM_S32 nDirEntries);


#endif

