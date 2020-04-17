/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_file.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 声明演示程序用到的文件解析和读写接口函数。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEMO_FILE_H
#define MAQUE_DEMO_FILE_H


typedef enum 
{
	XM_VALUE_TYPE_INT,  /* 整型 */
	XM_VALUE_TYPE_HEX,  /* 16进制数 */
	XM_VALUE_TYPE_STRING, /* 字符串 */
	XM_VALUE_TYPE_NR
} XmValueType_e;

/*
这个结构体用于从文件中获取字符串pString 对应的类型为eValType指定的最大长度为
valSizeMax的值。XM_VALUE_TYPE_INT和XM_VALUE_TYPE_HEX类型的数，在文件中保存的是字符
串，获取后存到pValue中的是整型数值。具体用法参考：LibXmComm_File_parse512 ()。
*/
typedef struct
{
	XM_CHAR *pString;  /*调用者赋值，指明需从文件中获取值的字符串。*/
	XM_VOID *pValue;   /*调用者分配空间，将从文件中获取的字符串pString对应的值保存在其中。*/
	XmValueType_e eValType;  /*从文件中获取的字符串对应的值类型。INT和HEX类型字符串最后都转换
							  *成数值，保存到pValue中。其定义，参考：xmcomm_struct.h。
							  */
	XM_U32 valSizeMax;	/*调用者给pValue分配的空间大小。*/
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
输入参数：pDirName：目录路径名，可以是相对，也可以是绝对路径。
	eDirEntryType：需要获取的目录项类型，指定为DIR_ENTRY_TYPE_NONE或DIR_ENTRY_TYPE_NR，都将获取所有文件和目录。
	pDirEntCompFuncPtr: 指定目录或文件名排序算法。例如: alphasort，versionsort等。为NULL时，默认使用versionsort。
				注，alphasort按字母或数字顺序排列；versionsort按字母和数字整体大小排列。例如: a1001.txt, a20.txt
					两个文件名，按alphasort排序方法，a1001.txt在前，按versionsort方法，a20.txt在前。
					用户也可以自己开发排序算法，要求:a > b，返回正数； a == b，返回0；a < b，返回负数。
输出参数：pppstDirEntList：保存获取的文件或目录名。由函数内部分配内存，调用成功，使用后，调用者负责释放内存。
					注意: 它是3维指针。
返回：成功：XM_SUCCESS；失败：XM_FAILURE。
函数功能：调用C库函数scandir()，获取pDirName目录下的文件或目录。成功后，给pppstDirEntList分配内存，并把获取的
	目录项保存在pppstDirEntList[i]中。处理后，调用者负责释放pppstDirEntList所指的内存。
	注意: pppstDirEntList是3维指针。调用者需要先释放pppstDirEntList[i]内存，再释放pppstDirEntList。例如
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
	也可以处理ppstDirEntList后，调用函数MaQue_Demo_Dir_freeDirEntryList()一次性释放。
*/
XM_S32 MaQue_Demo_Dir_getDirOrFileName(XM_CHAR *pDirName, DirEntryType_e eDirEntryType, 
	struct dirent ***pppstDirEntList, DirEntComp_FuncPtr pDirEntCompFuncPtr);

/*
输入参数：ppstDirEntList：需要释放的目录项列表，由MaQue_Demo_Dir_getDirOrFileName()获得。
			nDirEntries：需要释放的目录项条数。
输出参数：无。
返回：成功：XM_SUCCESS；失败：XM_FAILURE。
函数功能：逐个释放ppstDirEntList[i],最后释放ppstDirEntList本身。
*/
XM_S32 MaQue_Demo_Dir_freeDirEntryList(struct dirent **ppstDirEntList, XM_S32 nDirEntries);


#endif

