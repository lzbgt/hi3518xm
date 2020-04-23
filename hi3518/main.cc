#include <iostream>
#include <string>
#include <chrono>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include "utils.h"
#include "ref-memory.hpp"
#include <maque_system.h>
#include <maque_video_enc.h>
#include <maque_time.h>
#include "common.h"
#include "ntp.h"
#include <time.h>
#include "raw_tcp.h"
#include <evpacket.h>
#include "smart.h"
#include "motion.h"
#include <thread>

using namespace std;

//
//#define ENALBE_RECORD
#define NUM_IFRAME_PICK 2
#define NUM_MAX_QUEQUE_SIZE 60
#define NUM_MAX_PACKET_BYTES 1000

static unsigned long long frameCntTotal = 0;
static unsigned long long frameCntIframe = 0;
static unsigned long long frameCntPframe = 0;
static unsigned long long frameCntIframePrev = 0;
static bool bPFrameAvail = false;
static bool bAvailable = false;
static bool enablePush = false;

static queue<DataItem> _frameQueue;
static mutex _mutFrame;
static condition_variable _condFrame;
static Notifier _notiFrame = {&_mutFrame, &_condFrame};
static CallBackArg args = {nullptr, &_notiFrame, &_frameQueue};
static int raw_socket_ = 0;
static bool bGotTime = false;

void clean_up(int sig)
{
    spdlog::warn("clearn up called, with sig: {}", sig);
    if (args.recFD) {
        fclose(args.recFD);
    }

    if (raw_socket_) {
        ::close(raw_socket_);
    }

    // while (args.dataq->size() > 0) {
    //     DataItem elem = args.dataq->front();
    //     args.dataq->pop();
    //     MaQueVideoEncFrameInfo_s *pMem = (MaQueVideoEncFrameInfo_s *)elem.ud;
    //     MaQue_Demo_Mem_release(pMem->handleMem);
    // }

    //stop_md_bd();
    LibXmMaQue_System_destroy();
    exit(1);
}

MaQueVideoEncodeCfg_s *init_stream_cfg(MaQueVideoEncodeCfg_s *cfg)
{
    cfg->eVidComp = -1;
    cfg->eCapSize = -1;
    cfg->eBitrateCtrl = -1;
    cfg->eImageQuality = -1;
    cfg->nFps = -1;
    cfg->nBitRate = -1;
    cfg->nIFrameInterval = -1;
    cfg->iQpMin = -1;
    cfg->iQpMax = -1;

    return cfg;
}

int32_t configure_stream(MaQueStreamChannel_e eStreamChn, MaQueVideoEncodeCfg_s *cfg)
{
    int32_t res;
    MaQueVideoEncodeCfg_s oldCfg;

    if (LibXmMaQue_VideoEnc_getCfg(0, eStreamChn, &oldCfg)) {
        spdlog::error("LibXmMaQue_VideoEncode_getCfg error");
        return -1;
    }

    if (cfg->eVidComp != -1) {
        oldCfg.eVidComp = cfg->eVidComp;
    }
    if (cfg->eCapSize != -1) {
        oldCfg.eCapSize = cfg->eCapSize;
    }
    if (cfg->eBitrateCtrl != -1) {
        oldCfg.eBitrateCtrl = cfg->eBitrateCtrl;
    }

    if (cfg->eImageQuality != -1) {
        oldCfg.eImageQuality = cfg->eImageQuality;
    }

    if (cfg->nFps != -1) {
        oldCfg.nFps = cfg->nFps;
    }
    if (cfg->nBitRate != -1) {
        oldCfg.nBitRate = cfg->nBitRate;
    }

    if (cfg->nIFrameInterval != -1) {
        oldCfg.nIFrameInterval = cfg->nIFrameInterval;
    }

    if (cfg->iQpMin > 0 && cfg->iQpMin <= 51 && cfg->iQpMax > 0 && cfg->iQpMax <= 51) {
        oldCfg.iQpMin = cfg->iQpMin;
        oldCfg.iQpMax = cfg->iQpMax;
        oldCfg.bUseUserQp = 1;
    }

    spdlog::info("main: fps={}", oldCfg.nFps);

    res = LibXmMaQue_VideoEnc_setCfg(0, eStreamChn, &oldCfg);
    if (!res) {
        memcpy(cfg, &oldCfg, sizeof(oldCfg));
    }

    return res;
}

//
XM_S32 cb_frame_proc(XM_VOID *pUserArg, MaQueVideoEncFrameInfo_s *frame)
{
    MaQueSystemTime_s *pstSysTime = &frame->stTimeStamp.stSysTime;
    CallBackArg *args = (CallBackArg *)pUserArg;
    struct timeval stTimeVal;
    int ret;

    //spdlog::info("new frame {}", frameCntTotal);
    frameCntTotal++;

    // stTimeVal.tv_sec = pstSysTime->day * 24 * 60 * 60 + pstSysTime->hour * 60 * 60
    // 		+ pstSysTime->minute * 60 + pstSysTime->second;
    // stTimeVal.tv_usec = pstSysTime->second * 1000;

#ifdef ENALBE_RECORD
    if (args->recFD) {
        fwrite(frame->pData, 1, frame->nDataLen, args->recFD);
    }
#endif

    if (enablePush && args->dataq && frame->eEncodeType == MAQUE_ENCODE_TYPE_H264) {
        lock_guard<mutex> lock(*args->noti->mut);
        if (args->dataq->size() >= NUM_MAX_QUEQUE_SIZE) {
            bAvailable = false;
            spdlog::warn("dataq full");
        }
        else if (args->dataq->size() <= NUM_MAX_QUEQUE_SIZE * 2 / 3) {
            bAvailable = true;
        }

        if (frame->eSubType == MAQUE_FRAME_SUBTYPE_I) {
            frameCntIframe++;
        }

        if (bAvailable && frame->pData && frame->nDataLen > 0) {
            timeval tv;
            //::gettimeofday(&tv,NULL);
            DataItem dt = {(char *)frame->pData - sizeof(evpacket_t), frame->nDataLen + sizeof(evpacket_t), (void *)frame};
            evpacket_t *pkt = (evpacket_t *)dt.buf;
            memset(pkt, 0, sizeof(*pkt));
            pkt->meta.magic[0] = 0xBE;
            pkt->meta.magic[1] = 0xEF;
            // TODO: get sn
            memcpy(pkt->meta.sn, "test_sn", 8);
            pkt->vpara.frame_type = htons(frame->eSubType + 1);
            pkt->vpara.packet_type = htons(frame->eEncodeType);
            pkt->vpara.res.width = htons(frame->nWidth);
            pkt->vpara.res.height = htons(frame->nHeight);
            //pkt->vpara.ts = tv;
            pkt->length = htonl(frame->nDataLen);

            //  if(frame->eSubType == MAQUE_FRAME_SUBTYPE_I && (frameCntIframe-1) % NUM_IFRAME_PICK == 0) {
            MaQue_Demo_Mem_addRef(frame->handleMem);
            args->dataq->push(std::move(dt));
            args->noti->cond->notify_all();
            frameCntIframePrev = frameCntIframe;
            bPFrameAvail = true;
            frameCntPframe = 0;
            spdlog::debug("=====\nframe meter ic: {}, tc: {}, len: {}, p avail", frameCntIframe, frameCntTotal, frame->nDataLen);
            // }else if(bPFrameAvail && frame->eSubType == MAQUE_FRAME_SUBTYPE_P && frameCntIframe == frameCntIframePrev){
            //     MaQue_Demo_Mem_addRef(frame->handleMem);
            //     args->dataq->push(std::move(dt));
            //     args->noti->cond->notify_all();
            //     ++frameCntPframe;
            //     spdlog::debug("=====\nframe meter pc: {}, tc: {}, len: {}, p avail", frameCntPframe, frameCntTotal, frame->nDataLen);
            //     // spdlog::info("pframe : {}, {}", frameCntIframe, frameCntIframePrev);
            // }else{
            //     //spdlog::error("pframe not avail: {}, {}", frameCntIframe, frameCntIframePrev);
            //     bPFrameAvail = false;
            // }
        }
        else {
            spdlog::warn("h264 not avail");
        }
    }

    MaQue_Demo_Mem_release(frame->handleMem);
    return XM_SUCCESS;
}

void frame_send_entry(void *args)
{
    CallBackArg *pvArg = (CallBackArg *)args;
    unsigned long long frameCnt = 0;
    while (1) {
        DataItem elem;
        {
            unique_lock<mutex> lk(*pvArg->noti->mut);
            pvArg->noti->cond->wait(lk, [pvArg] { return !pvArg->dataq->empty(); });
            if (pvArg->dataq->empty()) {
                this_thread::sleep_for(chrono::milliseconds(500));
                printf("dataq empty");
                continue;
            }
            elem = pvArg->dataq->front();
            //pvArg->dataq->pop();
            frameCnt++;
        }
        MaQueVideoEncFrameInfo_s *pMem = (MaQueVideoEncFrameInfo_s *)elem.ud;

        if (elem.size <= 0 || elem.buf == nullptr || (elem.buf + sizeof(evpacket_t)) == nullptr) {
            spdlog::error("invalid frame. addr: {0:x}, len: {0:d}", (uint32_t)elem.buf, elem.size);
            pvArg->dataq->pop();
            MaQue_Demo_Mem_release(pMem->handleMem);
            continue;
        }

        // send frame
        if (elem.size > 0) {
            char *ptr = elem.buf;
            size_t sent = 0;
            // while((elem.size -= sent) > 0 &&
            //     (sent = ::send(raw_socket_, (ptr+=sent), elem.size >
            //         NUM_MAX_PACKET_BYTES? NUM_MAX_PACKET_BYTES:elem.size, 0)) > 0) {
            //     //
            // }
            while (elem.size > 0) {
                ptr += sent;
                //sent = ::send(raw_socket_, ptr, elem.size > NUM_MAX_PACKET_BYTES? NUM_MAX_PACKET_BYTES:elem.size, 0);
                sent = ::send(raw_socket_, ptr, elem.size, 0);
                if (sent <= 0) {
                    break;
                }
                elem.size -= sent;
            }

            if (sent < 0) {
                spdlog::error("faile to send");
                exit(1);
            }
        }
        else {
            spdlog::error("size error or not got time");
        }

        MaQue_Demo_Mem_release(pMem->handleMem);
        pvArg->dataq->pop();
    }
}

//
int main(int argc, char *argv[])
{
    int ret = XM_SUCCESS;
    spdlog::info("test on hi3518ev300");
    char *host = "192.168.55.104", *port = "7123";
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
        enablePush = true;
    }

    if (argc == 4) {
        spdlog::set_level(spdlog::level::debug);
    }
    // time_t stm;
    // if(getNtpTime(&stm) >=0){
    //     if(stime(&stm) < 0){
    //         spdlog::error("failed to set system time");
    //     }else{
    //         spdlog::info("ntp got time");
    //         bGotTime = true;
    //     }
    // }

    MaQueStartParam_s startParam = {0, {MAQUE_VIDEO_COMPRESS_H264, MAQUE_VIDEO_COMPRESS_H265}, "/mnt/sd/Config/"};
    ret = LibXmMaQue_System_startUp(&startParam);
    spdlog::info("ret: {}", ret);

    //
    MaQueMemoryApi_s memApi = {MaQue_Demo_Mem_alloc, MaQue_Demo_Mem_release, MaQue_Demo_Mem_addRef, MaQue_Demo_Mem_setLength};
    ret = LibXmMaQue_Mem_init(&memApi);
    spdlog::info("mem ret: {}", ret);

    MaQueSystemTime_s tm;
    LibXmMaQue_Time_getCurrentTime(&tm);
    LibXmMaQue_Time_print(&tm, "NULL");

    const char *testUrl = "rtsp://evcloud.ilabservice.cloud/test_pusher";

    //
    MaQueCodeAbilities_s capb;
    ret = LibXmMaQue_VideoEnc_getAbilities(0, &capb);
    spdlog::info("abt ret: {}", ret);
    if (XM_SUCCESS == ret) {
        spdlog::info(fmt::format("capbilities: supported codecs {0:#b}, \
            max fhd/s {0:d}, max res: {0:d}, per ch mres: {0:d}, {0:d}, {0:d}, {0:d}",
                                 int(capb.videoEncTypeMask),
                                 int(capb.maxEncPowerX1080P), int(capb.eDecImageSizeMax), int(capb.astVidEncChnAbility[0].eCapSizeMax),
                                 int(capb.astVidEncChnAbility[1].eCapSizeMax), int(capb.astVidEncChnAbility[2].eCapSizeMax), int(capb.astVidEncChnAbility[3].eCapSizeMax)));
    }

    //
    MaQueVideoEncodeCfg_s cfg;
    init_stream_cfg(&cfg);
    cfg.nFps = 15;
    cfg.eIFrmIntvType = IFRAME_INTV_TYPE_TIME;
    cfg.nIFrameInterval = 10;
    cfg.eImageQuality = MAQUE_IMG_QUALITY_BETTER;
    cfg.eVidComp = MAQUE_VIDEO_COMPRESS_H264;
    cfg.nBitRate = 200 * 8; // 200KB
    ret = configure_stream(0, &cfg);
    spdlog::info("cfg stream ret: {}", ret);

//
#ifdef ENALBE_RECORD
    const char *const recFilePath = "/mnt/sd/record/rec.264";
    FILE *recFD = fopen(recFilePath, "w+");
    if (!recFD) {
        spdlog::error("fopen() Failed: {}", recFilePath);
        LibXmMaQue_System_destroy();
        exit(0);
    }

    args.recFD = recFD;
#endif

    ret = LibXmMaQue_VideoEnc_startStream(0, 0, cb_frame_proc, (void *)&args);
    if (XM_SUCCESS == ret) {
        spdlog::info("created record task successfully");
    }

    signal(SIGINT, clean_up);
    //signal(SIGTERM, clean_up);
    signal(SIGKILL, clean_up);

    if (enablePush) {
        ret = raw_connect(host, port, &raw_socket_);
        if (ret < 0 || raw_socket_ <= 0) {
            spdlog::error("failed to create socket");
            exit(1);
        }
        thread thPush = thread(frame_send_entry, &args);
        spdlog::info("sizeof pkt header {}, sizeof tv {}", sizeof(evpacket_t), sizeof(timeval));
        thPush.detach();
    }

    start_md_bd(&args);

    thread thSmart = thread(maq_smart_task_entry, &args);
    thSmart.join();
}