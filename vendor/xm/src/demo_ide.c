
#include "demo_inc.h"
#include "demo_ide.h"

XM_S32 OnConsoleIde(XM_S32 argc, XM_CHAR** argv)
{
	if (argc < 2)
	{
help:
		printf("***********************************************ide help***********************************************\n");
		printf("ide -t [type] [dev] :  type: 1/2\n");
		printf("***********************************************ide help***********************************************\n");
		return 0;
	}

	MaQueMakeFs_s stMkFs;
	XM_S32 fsType = -1;
	XM_CHAR aDev[64] = {0};
	XM_S32 res;

	memset(&stMkFs, 0x0, sizeof(stMkFs));
	memset(&aDev, 0x0, sizeof(aDev));
	
	switch (argv[1][1])
	{
		case 't':
			if (argv[2])
			{
				fsType = atoi(argv[2]);
			}

			if (argv[3])
			{
				strncpy(aDev, argv[3], strlen(argv[3]));
			}
			break;
		default:
			goto help;
			break;
	};

	if (FS_TYPE_FAT32 != fsType
		&& FS_TYPE_EXFAT != fsType)
	{
		ERROR_PRINTF("fstype error\n");
		return XM_FAILURE;
	}

	INFO_PRINTF("dev : %s\n", aDev);

	stMkFs.eFsType = fsType;
	stMkFs.pDev = aDev;

	INFO_PRINTF("stMkFs.dev : %s\n", stMkFs.pDev);

	res = LibXmMaQue_Ide_makeFs(&stMkFs);
	if (XM_SUCCESS == res)
	{
		INFO_PRINTF("LibXmMaQue_Ide_makeFs Ok\n");
	}
	else
	{
		ERROR_PRINTF("LibXmMaQue_Ide_makeFs Failed!!!\n");
	}
	
	return XM_SUCCESS;
}

static XM_S32 Demo_Ide_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "ide", strlen("ide")))
	{
		MaQue_Demo_Console_OnLine((XM_CHAR*)pData, OnConsoleIde);
		
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Ide_start(XM_VOID)
{
	ConsoleCallback_s stCallback;

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Ide_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);

	return XM_SUCCESS;
}


