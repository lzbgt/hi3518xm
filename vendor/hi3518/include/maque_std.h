
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2018. All rights reserved. 
 * �ļ���: maque_std.h
 * ����@���: XM R&D @02479
 * �汾: V1.00
 * ��������: 2019.02.18
 * ��������: �����˳��õ�C��ͷ�ļ���������XmMaQue��һ�𷢲�������libxmmaque_api.h
 *		���������ⲿ������˵���ѱ��ļ����ƹ�ȥ��������ֻ��#include "libxmmaque_api.h"
 *		���ɣ����ص���������
 *
 * �޸���ʷ:  
 ** 1.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: �״δ��� ��
 ** 2.����: 2019.02.18
 ** �޸���: XM R&D @02479
 ** �޸�����: ����˻����档
 *************************************************/


#ifndef MAQUE_STD_H
#define MAQUE_STD_H

/*
 * must define before include
 */
#define _LARGEFILE_SOURCE		1
#define _LARGEFILE64_SOURCE		1
#define _FILE_OFFSET_BITS		64
#define _GNU_SOURCE

#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sched.h>
#include <termios.h> 
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/shm.h>
#include <sys/mount.h>
#include <sys/sysmacros.h>
#include <sys/poll.h>
#include <linux/types.h>
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <linux/major.h>
#include <linux/raw.h>
#include <linux/unistd.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <sys/sysinfo.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>
//#include <linux/wireless.h>
#include <net/if.h>
#include <net/if_arp.h>

#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <linux/mii.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <linux/un.h>
#include <linux/netlink.h>
#include <sys/prctl.h>

#include <sys/reboot.h>
#include <dlfcn.h>


#endif

