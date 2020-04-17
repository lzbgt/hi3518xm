
#ifndef MAQUE_MOTOR_H
#define MAQUE_MOTOR_H


#ifdef __cplusplus
extern "C" {
#endif

/* motor action */
#define MOTOR_MOVE_LEFT		1    
#define MOTOR_MOVE_RIGHT    2 
#define MOTOR_MOVE_UP       4 
#define MOTOR_MOVE_DOWN		8 
#define MOTOR_MOVE_LEFTUP (MOTOR_MOVE_LEFT | MOTOR_MOVE_UP)
#define MOTOR_MOVE_LEFTDOWN (MOTOR_MOVE_LEFT | MOTOR_MOVE_DOWN)
#define MOTOR_MOVE_RIGHTUP (MOTOR_MOVE_RIGHT | MOTOR_MOVE_UP)
#define MOTOR_MOVE_RIGHTDOWN (MOTOR_MOVE_RIGHT | MOTOR_MOVE_DOWN)

#define MOTOR_IDLE      0xffff

/*******************************************************************
 *	����: �������
 *  ���أ�	0 �ɹ�
 *        <0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_create(XM_VOID);

/*******************************************************************
 *	����: ���ٵ��
 *  ���أ�	0 �ɹ�
 *        <0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_destroy(XM_VOID);

/*******************************************************************
 *	����: ��ȡ��󲽳�
 *  ���أ�	0 �ɹ�
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_getMaxSteps(XM_U32 *pX, XM_U32 *pY);

/*******************************************************************
 *	����: ��ȡ�������
 *  ����:[in]action ת������
 *       [in]xspeed x���ٶ�[0,8]
 *       [in]yspeed y���ٶ�[0,8]
 *       [in]xstep x�Ჽ��[0,  x_stepmax]
 *       [in]ystep y�Ჽ��[0,  y_stepmax]
 *  ���أ�	0 �ɹ�
 *        <0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_move(XM_U32 action, XM_U32 xSpeed, XM_U32 ySpeed, XM_U32 xStep, XM_U32 yStep); 

/*x [0, x_stepmax] y [0, y_stepmax], xspeed(0,52], yspeed(0,52]*/
XM_S32 LibXmMaQue_Motor_gotoPreset(XM_U32 x, XM_U32 y, XM_U32 xSpeed, XM_U32 ySpeed);

/*******************************************************************
 *	����: ֹͣ���
 *  ���أ�	0 �ɹ�
 *        <0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_stop(XM_VOID);

/*******************************************************************
 *	����: ��ȡ�������
 *  ����:[in]action ת������
 *       [in]x x������
 *       [in]y y������
 *  ���أ�	0 �ɹ�
 *        <0 ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_getPostion(XM_U32 *pAction, XM_U32 *pX, XM_U32 *pY);

/*******************************************************************
 *	����:���õ�ǰ���꣬һ���������豸������ʱ�����ϴα������������꣬�����������¶�λ
 *  ����:[in]x x������
 *       [in]y y������
 *  ���أ�	0 �ɹ�
 *        <0  ʧ��
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_setPostion(XM_U32 x, XM_U32 y); 


/*******************************************************************
 *	����: �ƶ�����0,0��,���û���Լ죬����������ӿ�����ʼ������
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_setZero(XM_VOID);

#ifdef __cplusplus
}
#endif


#endif


