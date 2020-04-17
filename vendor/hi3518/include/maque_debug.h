
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_debug.h
 * 作者@编号: chenwenrong@02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 定义通用的调试宏。这些宏供外部使用。
 *		注意:DEBUG_PRINTF_CLOSE默认是没有定义的，在编译时，给xm_make加debug=no选项
 *			可定义。
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: chenwenrong@02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: chenwenrong@02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef MAQUE_DEBUG_H
#define MAQUE_DEBUG_H

/*
		打印宏							等级		用法													颜色
DEBUG_PRINTF_INTERNAL(string,args...)	低		代码调试打印，它只被DEBUG_PRINTF应用。默认是编译到库中的，
												可以通过xm_make的选项[debug=no]不编译到库中。 用法请参考
												表格后面具体说明。											粉色
INFO_PRINTF(string,args...)				中		用于信息提示。												蓝色
ERROR_PRINTF(string,args...)			高		用于程序出错时打印。										红色
SIMPLE_PRINTF(string,args...)			中		类似INFO_PRINTF，但没有文件名，函数名和行号打印。			蓝色
*/
/*
DEBUG_PRINTF_INTERNAL用法具体说明：
	一般每个库中都有"库名_inc.h"文件，这个文件需要所有.c文件包含。里面应包含所有.c文件都需要的头文件，
结构体，宏等。被各个库用于调试打印的宏DEBUG_PRINTF，最好定义在此文件中。
*/

/*
详细说明，参考: <雄迈LibXmComm库用户指南>。
*/

#ifndef DEBUG_PRINTF_CLOSE

#define DEBUG_PRINTF_INTERNAL(string,args...) printf("\033[35m""%s(%d) [%s]: "string"\033[0m",__FILE__,__LINE__,__FUNCTION__,##args)

#else

#define DEBUG_PRINTF_INTERNAL(string,args...) //printf("\033[35m""%s(%d) [%s]: "string"\033[0m",__FILE__,__LINE__,__FUNCTION__,##args)

#endif


#define INFO_PRINTF(string,args...)  printf("\033[34m""%s(%d) [%s]: "string"\033[0m",__FILE__,__LINE__,__FUNCTION__,##args)
#define ERROR_PRINTF(string,args...) fprintf(stderr,"\033[31m""%s(%d) [%s]: "string"\033[0m",__FILE__,__LINE__,__FUNCTION__,##args)
#define SIMPLE_PRINTF(string,args...)  printf("\033[34m"string"\033[0m", ##args)

#if 0
include <syslog.h>                                           
#include <stdarg.h> 

#define MODULE_NAME "LibXmMaQue"
#define DEBUG_PRINTF(string,args...) do{openlog(MODULE_NAME, LOG_PID|LOG_CONS, LOG_USER);syslog(LOG_DEBUG,"%s(%d) [%s]: "string,__FILE__,__LINE__,__FUNCTION__,##args);closelog(); }while(0)
#define ERROR_PRINTF(string,args...) do{openlog(MODULE_NAME, LOG_PID|LOG_CONS, LOG_USER);syslog(LOG_ERR,"%s(%d) [%s]: "string,__FILE__,__LINE__,__FUNCTION__,##args);closelog(); }while(0)
#endif

#endif

