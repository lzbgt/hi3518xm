
#include "demo_inc.h"
#include "demo_motor.h"

typedef struct
{
	XM_U32 xSpeed;
	XM_U32 ySpeed;
} MotorMgr_s;

static MotorMgr_s g_stMotorMgr =
{
	.xSpeed = 52,
	.ySpeed = 52,
};

XM_VOID WaitMotorIdle(XM_VOID)
{
	XM_U32 action,x,y;

	usleep(100*1000);

	while (1)
	{
		if (0 == LibXmMaQue_Motor_getPostion(&action,&x,&y))
		{
			if (MOTOR_IDLE == action)
			{
				break;
			}
			else
			{
				usleep(100*1000);
			}
		}
	}

}

XM_VOID InitMotor(XM_VOID)
{
	static XM_S32 first_startup = 1;//��ģ�⣬ʵ�������ϵ磬��Ҫ���浽flash 

	DEBUG_PRINTF("InitMotor start.\n");
	if (LibXmMaQue_Motor_create() < 0)
	{
		ERROR_PRINTF("LibXmMaQue_MotorCreate.\n");
	}

	//�����0����'��λ����'������£�ʹ������ṹ��λ,��һ��������Ҫ�����0
	//�´��������¼�ϴε�ͣ��������㣬�´��ϵ��ͨ��LibXmMaQue__MotorSetPostion����
	//δSetPostion��δ����SetZero���������Ĭ������0��0��ʵ������ͣ��λ�ú����Ĭ��ֵ���ܲ�ƥ��
	if (first_startup)
	{//��һ������ 
		if (LibXmMaQue_Motor_setZero())
		{
			ERROR_PRINTF("LibXmMaQue_MotorSetZero.\n");
		}
		//sleep(10);//��֤�ص�0λ,��ʹ��GetPostion��ȡ���״̬��actionΪMOTOR_IDLE�ټ���������������ʱ�򵥴���
	}
	else
	{
		//�ǵ�һ������,�����ϴε�ͣ������ x:1500,y:1000
		if (LibXmMaQue_Motor_setPostion(1500,1000))
		{
			ERROR_PRINTF("LibXmMaQue_SetPostion.\n");
		}
	}

	WaitMotorIdle();

	DEBUG_PRINTF("InitMotor end.\n");
}

XM_VOID RunMotor(XM_VOID)
{
	XM_U32 maxStepX, maxStepY;
	XM_U32 action, x, y;

	DEBUG_PRINTF("TestPtz start.\n");

	if (LibXmMaQue_Motor_getMaxSteps(&maxStepX, &maxStepY))
	{
		ERROR_PRINTF("LibXmMaQue_MotorGetMaxSteps.\n");
	}

	LibXmMaQue_Motor_getPostion(&action,&x,&y);

	DEBUG_PRINTF("LibXmMaQue_MotorGetMaxSteps, maxStepX:%u. maxStepY:%u.position(%u,%u)\n",maxStepX,maxStepY,x,y);

	//postion����Ϊx:0,y:0������£������ת������Ч��			
	LibXmMaQue_Motor_move(MOTOR_MOVE_RIGHTUP, g_stMotorMgr.xSpeed, g_stMotorMgr.ySpeed, maxStepX, maxStepY);
	WaitMotorIdle();

	LibXmMaQue_Motor_move(MOTOR_MOVE_LEFTDOWN, g_stMotorMgr.xSpeed, g_stMotorMgr.ySpeed, maxStepX>>1, maxStepY>>1);
	WaitMotorIdle();

	LibXmMaQue_Motor_gotoPreset(0, 0,g_stMotorMgr.xSpeed, g_stMotorMgr.ySpeed);
	WaitMotorIdle();
	
}

XM_VOID DestoryMotor(XM_VOID)
{
	LibXmMaQue_Motor_stop();
	LibXmMaQue_Motor_destroy();	
}

XM_S32 OnConsoleMotor(XM_S32 argc, XM_CHAR** argv)
{
	if (argc < 2)
	{
help:
		printf("***********************************************motor help***********************************************\n");
		printf("motor -m [xx/-xx] :  xx/-xx horizontal move right/left xx step, vertical move up/down xx step\n");
		printf("motor -g [x] [y] :  goto positon(x,y) \n");
		printf("motor -z : SetZero \n");
		printf("motor -t : turn around Motor \n");
		printf("***********************************************motor help***********************************************\n");
		return 0;
	}
	XM_S32 x = 0;
	XM_S32 y = 0;
	XM_U32 action = 0;
	switch (argv[1][1])
	{
		case 'm':
			if (argv[2])
			{
				x = atoi(argv[2]);				
				if(x > 0)
				{
					action |= MOTOR_MOVE_RIGHT;
				}
				else if(x < 0)
				{
					action |= MOTOR_MOVE_LEFT;
				}
			}

			if (argv[3])
			{
				y = atoi(argv[3]);
				if(y > 0)
				{
					action |= MOTOR_MOVE_UP;
				}
				else if(y < 0)
				{
					action |= MOTOR_MOVE_DOWN;
				}
			}
			LibXmMaQue_Motor_move(action, g_stMotorMgr.xSpeed, g_stMotorMgr.ySpeed, abs(x), abs(y));
			WaitMotorIdle();
			//DEBUG_PRINTF("x %d,y %d\n",x, y);
			break;
		case 'g':
			if (argv[2])
			{
				x = abs(atoi(argv[2]));				
			}

			if (argv[3])
			{
				y = abs(atoi(argv[3]));
			}
			LibXmMaQue_Motor_gotoPreset(x, y, g_stMotorMgr.xSpeed, g_stMotorMgr.ySpeed);
			//DEBUG_PRINTF("x %d,y %d\n",x, y);
			WaitMotorIdle();
			break;
		case 'z':
			LibXmMaQue_Motor_setZero();
			WaitMotorIdle();
			break;
		case 't':
			RunMotor();
			break;
#if 0
		case 'a':
			if (argv[2])
			{
				XM_S32 positionX = 0;
				if (atoi(argv[2]) > 0)
				{
					//����Ƕ�ֱ��ת������ 0~180
					positionX = MotorAngleToMotorPresetX(atoi(argv[2]));
				}
				else
				{
					//����Ƕ�ת����Դ��λ�ĽǶȲ���,0~180
					positionX = MotorAngleToMotorPresetX(AudioAngleToMotorAngle(atoi(argv[2])));
				}
				LibXmMaQue_Motor_gotoPreset(positionX, 100, 52, 52);
				DEBUG_PRINTF("GotoPreset X %d\n", positionX);
			}
			break;
#endif
		default:
			goto help;
			break;
	};
	return 0;
}

static XM_VOID *Demo_Motor_task(XM_VOID *pArg)
{
	InitMotor();

	return NULL;
}

static XM_S32 Demo_Motor_Console_callback(XM_VOID *pArg, XM_VOID *pData)
{
	pData = MaQue_Demo_String_trimLeftRightSpace(pData);
	
	if (!strncasecmp(pData, "motor", strlen("motor")))
	{
		MaQue_Demo_Console_OnLine((XM_CHAR*)pData, OnConsoleMotor);
		
		return XM_SUCCESS;	
	}

	return XM_SUCCESS;
}

XM_S32 MaQue_Demo_Motor_start(XM_VOID)
{
	XM_S32 bEnabled = 0;
	XM_S32 xSpeed = 0;
	XM_S32 ySpeed = 0;
	ConsoleCallback_s stCallback;
	StringValueGet_s astMotorMgrValGet[] = 
	{
		{"bEnabled:", &bEnabled, XM_VALUE_TYPE_INT, sizeof(bEnabled)},
		{"xSpeed:", &xSpeed, XM_VALUE_TYPE_INT, sizeof(xSpeed)},
		{"ySpeed:", &ySpeed, XM_VALUE_TYPE_INT, sizeof(ySpeed)},
	};

	g_stMotorMgr.xSpeed = xSpeed;
	g_stMotorMgr.ySpeed = ySpeed;

	stCallback.pCallbackArg = NULL;
	stCallback.pCallbackFuncPtr = Demo_Motor_Console_callback;

	MaQue_Demo_Console_registerCallback(&stCallback);
		
	{
		MaQue_Demo_File_parse512(MaQue_Demo_Common_getRefFile(), 
			"Motor", astMotorMgrValGet, sizeof(astMotorMgrValGet) / sizeof(StringValueGet_s));

		INFO_PRINTF("Motor bEnabled: %d\n", bEnabled);

		if (bEnabled)
		{
			MaQue_Demo_Common_createThread(Demo_Motor_task, NULL, "Motor_task");
		}
	}

	return XM_SUCCESS;
}


