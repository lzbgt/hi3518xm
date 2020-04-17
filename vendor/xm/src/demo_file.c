
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: demo_file.c
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.03.01
 * 功能描述: 实现演示程序用到的文件解析和读写接口函数。
 *
 * 修改历史:  
 ** 1.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.03.01
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/
 

#include "demo_inc.h"


XM_S32 MaQue_Demo_File_Line_parse(XM_CHAR *pLine, StringValueGet_s *pstStrValGet, XM_S32 nEntriesMax)
{
	XM_S32 i;
	XM_CHAR *p;

	//INFO_PRINTF("pLine: %s\n", pLine);
	
	for (i = 0; i < nEntriesMax; i++)
	{
		p = MaQue_Demo_String_trimLeftSpace(pLine);
		if (!p || !strlen(p))
		{
			continue;
		}
		
		if(strncasecmp(p, pstStrValGet[i].pString, strlen(pstStrValGet[i].pString)))
		{
			continue;	
		}

		p += strlen(pstStrValGet[i].pString);

		p = MaQue_Demo_String_trimLeftRightSpace(p);

		switch (pstStrValGet[i].eValType)
		{
			case XM_VALUE_TYPE_INT:
				*(XM_S32 *)pstStrValGet[i].pValue = strtol(p, NULL, 10); /* 注，strtol会跳过前面空格.*/
				break;

			case XM_VALUE_TYPE_HEX:
				*(XM_S32 *)pstStrValGet[i].pValue = strtol(p, NULL, 16); /* 注，strtol会跳过前面空格.*/
				break;

			case XM_VALUE_TYPE_STRING:
				strncpy(pstStrValGet[i].pValue, p, pstStrValGet[i].valSizeMax - 1);
				break;

			default:
				ERROR_PRINTF("Invalid eValType:%d <<ERROR>>\n", pstStrValGet[i].eValType);
				return XM_FAILURE;
		}
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_File_parse512(XM_CHAR *pFilePath, XM_CHAR *pModuleName,
			StringValueGet_s *pstStrValGet, XM_U32 nEntries)
{
	XM_CHAR aLine[512];
	FILE *pFile;
	XM_CHAR *p;
	XM_S32 bFound = 0;
	
	pFile = fopen(pFilePath, "r");
	if (!pFile)
	{
		ERROR_PRINTF("fopen() Failed:%s\n", pFilePath);
		return XM_FAILURE;
	}
	
	while (fgets(aLine, sizeof(aLine), pFile))
	{
		//INFO_PRINTF("aLine: %s\n", aLine);
		
		p = MaQue_Demo_String_trimLeftSpace(aLine);
		if (!p || !strlen(p))
		{
			continue;
		}

		//INFO_PRINTF("p: %s <<11111>>\n", p);
		
		if(strncasecmp(p, "Module:", strlen("Module:")))
		{
			continue;	
		}

		//INFO_PRINTF("p: %s <<22222>>\n", p);

		p += strlen("Module:");
		
		if (!strncasecmp(MaQue_Demo_String_trimLeftSpace(p), pModuleName, strlen(pModuleName)))
		{
			bFound = 1;
			INFO_PRINTF("Found Module: %s\n", pModuleName);
			break;
		}
	}

	if (!bFound)
	{
		INFO_PRINTF("Not Found Module: %s\n", pModuleName);
		return XM_FAILURE;
	}
	
	while (fgets(aLine, sizeof(aLine), pFile))
	{
		if (strlen(aLine) <= 0)
		{
			continue;
		}

		p = strstr(aLine, "Module:");
		if(p)
		{
			break;	
		}
		
		MaQue_Demo_File_Line_parse(aLine, pstStrValGet, nEntries);
	}

	fclose(pFile);

	return XM_SUCCESS;
} 

XM_S64 MaQue_Demo_File_getSize(XM_CHAR *pFilePath)
{
	XM_S32 res;
	struct stat stStat;
	XM_S32 fd;

	fd = open ((const XM_CHAR *)pFilePath, O_RDONLY);
	if (fd < 0)
	{
		ERROR_PRINTF("open() Failed, %s!\n", pFilePath);
		return XM_FAILURE;
	}

	res = fstat (fd, &stStat);
	if (res < 0)
	{
		ERROR_PRINTF("fstat() Failed, %s!\n", pFilePath);
		close(fd);
		return XM_FAILURE;
	}

	close(fd);
	
	DEBUG_PRINTF("File:%s, size = %d\n", pFilePath, (XM_S32)(stStat.st_size));

	return stStat.st_size;
}


XM_S32 MaQue_Demo_File_write(XM_S32 fd, XM_VOID *pBuf, XM_S32 count) 
{ 
	XM_S32 leftBytes;  
	XM_S32 writtenBytes; 
	XM_CHAR *pDat; 

	pDat = pBuf; 
	leftBytes = count; 

	while (leftBytes > 0) 
	{       
		writtenBytes = write(fd, pDat, leftBytes); 

		if(writtenBytes <= 0) 
		{            
			if(EINTR == errno || EAGAIN == errno)
			{
				writtenBytes = 0;
			}
			else             /* 其他错误，没有办法*/ 
			{
				return XM_FAILURE; 
			}
		}

		leftBytes -= writtenBytes; 
		pDat += writtenBytes;
	} 

	return count - leftBytes;
}

XM_S32 MaQue_Demo_File_read(XM_S32 fd, XM_VOID *pBuf, XM_S32 count) 
{ 
	XM_S32 leftBytes; 
	XM_S32 readBytes = 0; 
	XM_CHAR *pDat = pBuf; 

	leftBytes = count; 

	while (leftBytes > 0) 
	{ 
		readBytes = read(fd, pDat, leftBytes); 

		DEBUG_PRINTF("readBytes: %d\n", readBytes);

		if(readBytes < 0) 
		{ 
			if(EINTR == errno || EAGAIN == errno) 
			{
				readBytes = 0;
			}
			else 
			{
				return (count - leftBytes) > 0 ? (count - leftBytes) : XM_FAILURE; 
			}
		} 

		else if(0 == readBytes) 
		{
			break;
		}

		leftBytes -= readBytes; 
		pDat += readBytes; 
	}

	DEBUG_PRINTF("count: %d, leftBytes: %d\n", count, leftBytes);

	return (count - leftBytes); 
}


XM_S32 MaQue_Demo_File_writeSimply(XM_CHAR *pFilePath, XM_U8 *pBuf, XM_S32 count)
{
	XM_S32 res = 0;
	XM_S32 fd = -1;
		
	if (!pFilePath || !pBuf || count <= 0)
	{
		return XM_FAILURE;
	}

	if ((fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0)
	{
		ERROR_PRINTF("open() Failed: %s, Error: %s\n", pFilePath, strerror(errno));
		return XM_FAILURE;
	}

	if ((res = MaQue_Demo_File_write(fd, pBuf, count)) < 0)
	{
		ERROR_PRINTF("LibXmComm_File_write Failed, count:%d.\n", count);
		fsync(fd);
		close(fd);
		return XM_FAILURE;
	}

	fsync(fd);
	close(fd);

	return res;
}

XM_S32 MaQue_Demo_File_readSimply(XM_CHAR *pFilePath, XM_U8 *pBuf, XM_S32 count)
{
	XM_S32 res;
	XM_S32 fd = -1;
	
	if (!pFilePath || !pBuf || count <= 0)
	{
		return XM_FAILURE;
	}

	if ((fd = open(pFilePath, O_RDONLY)) < 0)
	{
		ERROR_PRINTF("open() Failed:%s\n", pFilePath);
		return XM_FAILURE;
	}
	
	if ((res = MaQue_Demo_File_read(fd, pBuf, count)) <= 0)
	{
		ERROR_PRINTF("LibXmComm_File_read() Failed, File: %s, res:%d\n", pFilePath, res);
		close(fd);
		return XM_FAILURE;
	}

	close(fd);

	return res;
}

static XM_S32 Dir_isDir(const struct dirent *pstDirEnt)
{
	struct stat stStatBuf;
	
	if (DT_UNKNOWN == pstDirEnt->d_type)
	{
		lstat(pstDirEnt->d_name, &stStatBuf);
		
		if(S_ISDIR(stStatBuf.st_mode))
		{
			return XM_TRUE;
		}
		else
		{
			return XM_FALSE;
		}
	}

	if (DT_DIR == pstDirEnt->d_type)
	{
		return XM_TRUE;
	}

	return XM_FALSE;
}

static XM_S32 Dir_selectDirEnt (const struct dirent *pstDirEnt)
{
	return Dir_isDir(pstDirEnt);
}

static XM_S32 Dir_selectFileEnt (const struct dirent *pstDirEnt)
{
	return !Dir_isDir(pstDirEnt);
}

XM_S32 MaQue_Demo_Dir_getDirOrFileName(XM_CHAR *pDirName, DirEntryType_e eDirEntryType,
	struct dirent ***pppstDirEntList, DirEntComp_FuncPtr pDirEntCompFuncPtr)
{
	XM_S32 dirCount;
	DirEntComp_FuncPtr pCompFuncPtr = versionsort;

	if (pDirEntCompFuncPtr)
	{
		pCompFuncPtr = pDirEntCompFuncPtr;
	}

	if (DIR_ENTRY_TYPE_DIR == eDirEntryType)
	{
		dirCount = scandir(pDirName, pppstDirEntList, Dir_selectDirEnt, pCompFuncPtr);
	}
	else if (DIR_ENTRY_TYPE_FILE == eDirEntryType)
	{
		dirCount = scandir(pDirName, pppstDirEntList, Dir_selectFileEnt, pCompFuncPtr);
	}
	else
	{
		dirCount = scandir(pDirName, pppstDirEntList, NULL, pCompFuncPtr);
	}
	
    if (dirCount < 0)
    {
    	ERROR_PRINTF("scandir Failed, Dir: %s.\n", pDirName);
		return XM_FAILURE;
    }

	return dirCount;
}

XM_S32 MaQue_Demo_Dir_freeDirEntryList(struct dirent **ppstDirEntList, XM_S32 nDirEntries)
{
	XM_S32 i;

	if (!ppstDirEntList || nDirEntries <= 0)
	{
		ERROR_PRINTF("if (!ppstDirEntList || nDirEntries: %d <= 0).\n", nDirEntries);
		return XM_FAILURE;
	}
	
	for (i = 0; i < nDirEntries; i++) 
	{
		//DEBUG_PRINTF("%s\n", ppstDirEntList[i]->d_name);
	    free(ppstDirEntList[i]);
	}

	free(ppstDirEntList);

	return XM_SUCCESS;
}

