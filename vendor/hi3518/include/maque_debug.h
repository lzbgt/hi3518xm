
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * �ļ���: maque_debug.h
 * ����@���: chenwenrong@02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: ����ͨ�õĵ��Ժꡣ��Щ�깩�ⲿʹ�á�
 *		ע��:DEBUG_PRINTF_CLOSEĬ����û�ж���ģ��ڱ���ʱ����xm_make��debug=noѡ��
 *			�ɶ��塣
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: chenwenrong@02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: chenwenrong@02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_DEBUG_H
#define MAQUE_DEBUG_H

/*
		��ӡ��							�ȼ�		�÷�													��ɫ
DEBUG_PRINTF_INTERNAL(string,args...)	��		������Դ�ӡ����ֻ��DEBUG_PRINTFӦ�á�Ĭ���Ǳ��뵽���еģ�
												����ͨ��xm_make��ѡ��[debug=no]�����뵽���С� �÷���ο�
												���������˵����											��ɫ
INFO_PRINTF(string,args...)				��		������Ϣ��ʾ��												��ɫ
ERROR_PRINTF(string,args...)			��		���ڳ������ʱ��ӡ��										��ɫ
SIMPLE_PRINTF(string,args...)			��		����INFO_PRINTF����û���ļ��������������кŴ�ӡ��			��ɫ
*/
/*
DEBUG_PRINTF_INTERNAL�÷�����˵����
	һ��ÿ�����ж���"����_inc.h"�ļ�������ļ���Ҫ����.c�ļ�����������Ӧ��������.c�ļ�����Ҫ��ͷ�ļ���
�ṹ�壬��ȡ������������ڵ��Դ�ӡ�ĺ�DEBUG_PRINTF����ö����ڴ��ļ��С�
*/

/*
��ϸ˵�����ο�: <����LibXmComm���û�ָ��>��
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

