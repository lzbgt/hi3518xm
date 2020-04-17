
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2018. All rights reserved. 
 * 文件名: maque_std.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: 包含了常用的C库头文件。它将随XmMaQue库一起发布。它被libxmmaque_api.h
 *		包含，对外部程序来说，把本文件复制过去，代码中只需#include "libxmmaque_api.h"
 *		即可，不必单独包含。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
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

