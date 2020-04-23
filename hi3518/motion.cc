extern "C" {
#include <maque_type.h>
#include <maque_osd.h>
#include <maque_motion_detect.h>
#include <maque_debug.h>
}

#include <stdio.h>
#include <spdlog/spdlog.h>

typedef struct {
    MaQueMdParam_s stMdParam;
} MdModParam_s;

typedef struct {
    MaQueBdParam_s stBdParam;;
} BdModParam_s;

typedef struct {
    XM_S32 bMdEnabled;
    XM_S32 bBdEnabled;;
} MdBdMgr_s;


static XM_S32 cb_motion_detect(XM_VOID *pUserArg, MaQueMdAlarm_s *pstMdAlarm)
{
    spdlog::info("Detected, eAlarmType: {}, alarmState: {}", pstMdAlarm->eAlarmType, pstMdAlarm->alarmState);
    return 0;
}

static XM_S32 cb_blind_detect(XM_VOID *pUserArg, MaQueBdResult_s *pstBdRes)
{
    static int prevState = 0;

    if(pstBdRes->state != prevState){
        if(pstBdRes->state == 1) {
            spdlog::info("The Camera is blind");
        }else{
            spdlog::info("The Camera is blind recovered");
        }
        prevState = pstBdRes->state;
    }
    return 0;
}

void start_md_bd(void *args)
{
    MdModParam_s mdParam = { 0 };
    BdModParam_s bdParam = { 0 };

    // TODO: params setting from args
    mdParam.stMdParam.bEnabled = 1;
    mdParam.stMdParam.x = 58;
    mdParam.stMdParam.y = 48;
    mdParam.stMdParam.w = 352 - 58*2;
    mdParam.stMdParam.h = 288 - 48*2;
    mdParam.stMdParam.eMdAlarmlevel = 2; // [1, 6]
    bdParam.stBdParam.bEnabled = 1;
    bdParam.stBdParam.eBdAlarmLevel = 2;

    if (mdParam.stMdParam.bEnabled) {
        LibXmMaQue_MD_create(0);
        LibXmMaQue_MD_setParam(0, &mdParam.stMdParam);
        LibXmMaQue_MDAlarm_register(0, cb_motion_detect, NULL);
    }

    if (bdParam.stBdParam.bEnabled) {
        LibXmMaQue_BD_create(0);
        LibXmMaQue_BD_setParam(0, &bdParam.stBdParam);
        LibXmMaQue_BD_register(0, cb_blind_detect, NULL);
    }
}


void stop_md_bd(){
    LibXmMaQue_MD_destroy(0);
    LibXmMaQue_BD_destroy(0);
}


