/************************************************* 
 * Copyright ? Xiongmai Information Technologies Co., Ltd. 2019-2019. All rights reserved. 
 * 文件名: maque_type.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 主要定义统一的数据类型。
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.……
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
#define XM_FAILURE  (-1)  /* 致命错误。*/
#define XM_AGAIN  (-2) /* 操作失败，可以重试。*/

#define XM_BIT_MASK(bit)		(1 << (bit))

#define XM_SWAP16(data)		(((data & 0xFF) << 8) | ((data >> 8) & 0xFF))
#define XM_SWAP32(data)		(((data & 0xFF) << 24) | (((data >> 8) & 0xFF) << 16)	\
		| (((data >> 16) & 0xFF) << 8) | (((data >> 24) & 0xFF)))


//Notice: typedef XM_VOID(*FreeNodeFunc)(XM_VOID *) -- is pointer
//typedef XM_VOID FreeNodeFunc(XM_VOID *) also OK --not pointer

typedef XM_U32 (*U32_VoidP_FuncPtr)(XM_VOID *);   /* 返回U32，输入XM_VOID*  */
typedef XM_S32 (*S32_VoidP_FuncPtr)(XM_VOID *);		/* 返回S32，输入XM_VOID*  */
typedef XM_S32 (*S32_VoidP_VoidP_FuncPtr)(XM_VOID *, XM_VOID *);		/* 返回S32，输入XM_VOID*，输入XM_VOID*  */
typedef XM_S32 (*S32_VoidP_VoidP_U32_FuncPtr)(XM_VOID *, XM_VOID *, XM_U32);		/* 返回S32，输入XM_VOID*，输入XM_VOID*，输入U32 */
typedef XM_VOID (*Void_VoidP_FuncPtr)(XM_VOID *);	/* 返回XM_VOID，输入XM_VOID*  */
typedef XM_VOID *(*VoidP_VoidP_FuncPtr)(XM_VOID *);	/* 返回XM_VOID*，输入XM_VOID*  */

typedef XM_S32 (*S32_VoidP_U8P_S32_FuncPtr)(XM_VOID *, XM_U8 *, XM_S32);		/* 返回S32，输入XM_VOID *，XM_U8*和S32 */
typedef XM_S32 (*S32_U32P_U32P_U32_FuncPtr)(XM_U32 *, XM_U32 *, XM_U32);		/* 返回S32，输入U32*、U32* 和U32 */

typedef XM_U32 (*S32_S32_FuncPtr)(XM_VOID *);   /* 返回U32，输入S32  */
typedef XM_U32 (*S32_U32_FuncPtr)(XM_VOID *);   /* 返回U32，输入U32  */
typedef XM_S32 (*S32_U32_U32_FuncPtr)(XM_U32, XM_U32);		/* 返回S32，输入U32 和U32 */

typedef XM_S32 (*S32_ULP_ULP_U32_FuncPtr)(XM_UL *, XM_UL *, XM_U32);		/* 返回S32，输入UL*、UL* 和U32 */
typedef XM_S32 (*S32_UL_UL_FuncPtr)(XM_UL, XM_UL);		/* 返回S32，输入UL 和UL */

typedef XM_S32 (*S32_S32_CharPP_FuncPtr)(XM_S32, XM_CHAR **);		/* 返回S32，输入XM_S32 和XM_CHAR** */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MAQUE_TYPE_H */

