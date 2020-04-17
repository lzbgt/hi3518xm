
/************************************************* 
 * Copyright (C), Xiongmai Information Technologies Co., Ltd. 2018-2019. All rights reserved. 
 * 文件名: maque_env.h
 * 作者@编号: XM R&D @02479
 * 版本: V1.00
 * 创建日期: 2019.02.18
 * 功能描述: #include了libXmMaQue库中所有头文件，因此外部使用libXmMaQue库的
 *		程序只需包含这个头文件。
 *
 * 修改历史:  
 ** 1.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 首次创建 。
 ** 2.日期: 2019.02.18
 ** 修改者: XM R&D @02479
 ** 修改内容: 完成了基础版。
 *************************************************/


#ifndef LIBMAQUE_API_H
#define LIBMAQUE_API_H

#include "maque_std.h"
#include "maque_type.h"
#include "maque_struct.h"
#include "maque_debug.h"

#include "maque_aac.h"
#include "maque_alarm.h"
#include "maque_asr.h"
#include "maque_audio.h"

#include "maque_camera.h"
#include "maque_crc.h"
#include "maque_env.h"
#include "maque_flash.h"
#include "maque_gpio.h"
#include "maque_ide.h"
#include "maque_jpeg_dec.h"
#include "maque_jpeg_enc.h"
#include "maque_motion_detect.h"
#include "maque_mem.h"
#include "maque_misc.h"
#include "maque_mmz.h"
#include "maque_motor.h"

#include "maque_mp4.h"
#include "maque_net.h"
#include "maque_osd.h"
//#include "maque_play.h"
#include "maque_rs485.h"
#include "maque_system.h"

#include "maque_time.h"
#include "maque_track.h"
#include "maque_uvc.h"

#include "maque_video_enc.h"
#include "maque_video_out.h"

#include "maque_watchdog.h"
#include "maque_yuv.h"
#include "maque_wlan.h"
#include "maque_mtdver.h"

#include "maque_smart.h"

#endif

