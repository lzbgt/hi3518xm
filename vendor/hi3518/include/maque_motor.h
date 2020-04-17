
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
 *	功能: 创建电机
 *  返回：	0 成功
 *        <0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_create(XM_VOID);

/*******************************************************************
 *	功能: 销毁电机
 *  返回：	0 成功
 *        <0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_destroy(XM_VOID);

/*******************************************************************
 *	功能: 获取最大步长
 *  返回：	0 成功
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_getMaxSteps(XM_U32 *pX, XM_U32 *pY);

/*******************************************************************
 *	功能: 获取电机坐标
 *  传参:[in]action 转动方向
 *       [in]xspeed x轴速度[0,8]
 *       [in]yspeed y轴速度[0,8]
 *       [in]xstep x轴步长[0,  x_stepmax]
 *       [in]ystep y轴步长[0,  y_stepmax]
 *  返回：	0 成功
 *        <0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_move(XM_U32 action, XM_U32 xSpeed, XM_U32 ySpeed, XM_U32 xStep, XM_U32 yStep); 

/*x [0, x_stepmax] y [0, y_stepmax], xspeed(0,52], yspeed(0,52]*/
XM_S32 LibXmMaQue_Motor_gotoPreset(XM_U32 x, XM_U32 y, XM_U32 xSpeed, XM_U32 ySpeed);

/*******************************************************************
 *	功能: 停止电机
 *  返回：	0 成功
 *        <0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_stop(XM_VOID);

/*******************************************************************
 *	功能: 获取电机坐标
 *  传参:[in]action 转动方向
 *       [in]x x轴坐标
 *       [in]y y轴坐标
 *  返回：	0 成功
 *        <0 失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_getPostion(XM_U32 *pAction, XM_U32 *pX, XM_U32 *pY);

/*******************************************************************
 *	功能:设置当前坐标，一般是用来设备刚起来时设置上次保存下来的坐标，这样不用重新定位
 *  传参:[in]x x轴坐标
 *       [in]y y轴坐标
 *  返回：	0 成功
 *        <0  失败
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_setPostion(XM_U32 x, XM_U32 y); 


/*******************************************************************
 *	功能: 移动到（0,0）,如果没有自检，可以用这个接口来初始化坐标
 *******************************************************************/
XM_S32 LibXmMaQue_Motor_setZero(XM_VOID);

#ifdef __cplusplus
}
#endif


#endif


