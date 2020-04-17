
#include "demo_inc.h"

static XM_VOID *Demo_ResetLed_task(XM_VOID *pArg)
{
	XM_U32 value;
	XM_S32 ledStatusRGB[3];
	XM_U8 i = 0;

	INFO_PRINTF("---------------- ResetLed Start -----------------!\n");

	/* 初始化红，绿，蓝三色灯，如果板子无该三色灯，将不起作用。*/
	memset(ledStatusRGB, 0x0, sizeof(ledStatusRGB));
	LibXmMaQue_Misc_setLedState(ledStatusRGB[0], ledStatusRGB[1], ledStatusRGB[2]);

	while(1)
	{
		LibXmMaQue_Misc_readResetKey(&value);
		
		if (!value)
		{
			/* 依次点亮红，绿，蓝三色灯，如果板子无该三色灯，将不起作用。*/
			memset(ledStatusRGB, 0x0, sizeof(ledStatusRGB));
			ledStatusRGB[i % 3] = 1;
			LibXmMaQue_Misc_setLedState(ledStatusRGB[0], ledStatusRGB[1], ledStatusRGB[2]);


			/* 开关系统指示灯，如果板子无该灯，将不起作用。*/
			if (i & 0x1)
			{
				LibXmMaQue_Misc_openSystemLed(1);
			}
			else
			{
				LibXmMaQue_Misc_openSystemLed(0);
			}

			i++;

			if (i > 5)
			{
				LibXmMaQue_System_reboot();
			}
		}
		
		sleep(1);
	}

	return NULL;
}

XM_S32 MaQue_Demo_ResetLed_start(XM_VOID)
{
	LibXmMaQue_Misc_ResetLedInit();
	
	MaQue_Demo_Common_createThread(Demo_ResetLed_task, NULL, "ResetLed");

	return XM_SUCCESS;
}


