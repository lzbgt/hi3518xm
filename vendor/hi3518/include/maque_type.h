/************************************************* 
 * Copyright ? Xiongmai Information Technologies Co., Ltd. 2019-2019. All rights reserved. 
 * �ļ���: maque_type.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ��Ҫ����ͳһ���������͡�
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����
 *************************************************/

#ifndef MAQUE_TYPE_H
#define MAQUE_TYPE_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/
typedef char					XM_CHAR;

typedef unsigned char           XM_U8;
typedef unsigned short          XM_U16;
typedef unsigned int            XM_U32;
typedef unsigned long           XM_UL;

typedef signed char             XM_S8;
typedef short                   XM_S16;
typedef int                     XM_S32;
typedef long           			XM_L;

/*----------------------------------------------*
 * The fixed-point data type, used to represent float data in hardware calculations.*
 *----------------------------------------------*/
/*--u8bit---------------------------------------*/
typedef unsigned char           XM_U0Q8;
typedef unsigned char           XM_U1Q7;
typedef unsigned char           XM_U5Q3;

/*--u16bit---------------------------------------*/
typedef unsigned short          XM_U0Q16;
typedef unsigned short          XM_U4Q12;
typedef unsigned short          XM_U6Q10;
typedef unsigned short          XM_U8Q8;
typedef unsigned short          XM_U12Q4;
typedef unsigned short          XM_U14Q2;

/*--s16bit---------------------------------------*/
typedef short                   XM_S9Q7;
typedef short                   XM_S14Q2;
typedef short                   XM_S1Q15;

/*--u32bit---------------------------------------*/
typedef XM_U32            		XM_U22Q10;
typedef XM_U32            		XM_U25Q7;

/*--s32bit---------------------------------------*/
typedef XM_S32                  XM_S25Q7;
typedef XM_S32                  XM_S16Q16;

/*----------------------------------------------*
 * The fixed-point data type combine with FLAG bits.*
 *----------------------------------------------*/

/*8bits unsigned integer,4bits decimal fraction,4bits flag bits*/
typedef unsigned short          XM_U8Q4F4;

/*float*/
typedef float               	XM_FLOAT;
/*double*/
typedef double                  XM_DOUBLE;


#ifndef _M_IX86
    typedef unsigned long long  XM_U64;
    typedef long long           XM_S64;
#else
    typedef __int64             XM_U64;
    typedef __int64             XM_S64;
#endif
	
#define XM_VOID                 void

typedef XM_UL                XM_HANDLE;

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/

typedef enum {
    XM_FALSE = 0,
    XM_TRUE  = 1,
} XM_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define XM_NULL     0L
#define XM_SUCCESS  0
#define XM_FAILURE  (-1)  /* ��������*/
#define XM_AGAIN  (-2) /* ����ʧ�ܣ��������ԡ�*/

#define XM_BIT_MASK(bit)		(1 << (bit))

#define XM_SWAP16(data)		(((data & 0xFF) << 8) | ((data >> 8) & 0xFF))
#define XM_SWAP32(data)		(((data & 0xFF) << 24) | (((data >> 8) & 0xFF) << 16)	\
		| (((data >> 16) & 0xFF) << 8) | (((data >> 24) & 0xFF)))


//Notice: typedef XM_VOID(*FreeNodeFunc)(XM_VOID *) -- is pointer
//typedef XM_VOID FreeNodeFunc(XM_VOID *) also OK --not pointer

typedef XM_U32 (*U32_VoidP_FuncPtr)(XM_VOID *);   /* ����U32������XM_VOID*  */
typedef XM_S32 (*S32_VoidP_FuncPtr)(XM_VOID *);		/* ����S32������XM_VOID*  */
typedef XM_S32 (*S32_VoidP_VoidP_FuncPtr)(XM_VOID *, XM_VOID *);		/* ����S32������XM_VOID*������XM_VOID*  */
typedef XM_S32 (*S32_VoidP_VoidP_U32_FuncPtr)(XM_VOID *, XM_VOID *, XM_U32);		/* ����S32������XM_VOID*������XM_VOID*������U32 */
typedef XM_VOID (*Void_VoidP_FuncPtr)(XM_VOID *);	/* ����XM_VOID������XM_VOID*  */
typedef XM_VOID *(*VoidP_VoidP_FuncPtr)(XM_VOID *);	/* ����XM_VOID*������XM_VOID*  */

typedef XM_S32 (*S32_VoidP_U8P_S32_FuncPtr)(XM_VOID *, XM_U8 *, XM_S32);		/* ����S32������XM_VOID *��XM_U8*��S32 */
typedef XM_S32 (*S32_U32P_U32P_U32_FuncPtr)(XM_U32 *, XM_U32 *, XM_U32);		/* ����S32������U32*��U32* ��U32 */

typedef XM_U32 (*S32_S32_FuncPtr)(XM_VOID *);   /* ����U32������S32  */
typedef XM_U32 (*S32_U32_FuncPtr)(XM_VOID *);   /* ����U32������U32  */
typedef XM_S32 (*S32_U32_U32_FuncPtr)(XM_U32, XM_U32);		/* ����S32������U32 ��U32 */

typedef XM_S32 (*S32_ULP_ULP_U32_FuncPtr)(XM_UL *, XM_UL *, XM_U32);		/* ����S32������UL*��UL* ��U32 */
typedef XM_S32 (*S32_UL_UL_FuncPtr)(XM_UL, XM_UL);		/* ����S32������UL ��UL */

typedef XM_S32 (*S32_S32_CharPP_FuncPtr)(XM_S32, XM_CHAR **);		/* ����S32������XM_S32 ��XM_CHAR** */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MAQUE_TYPE_H */

