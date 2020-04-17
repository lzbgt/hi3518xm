/*
module: zmqhelper
description: 
author: Bruce.Lu <lzbgt@icloud.com>
update: 2019/08/23
*/

#ifndef __ZMQ_HELPER_H__
#define __ZMQ_HELPER_H__

#undef ZMQ_BUILD_DRAFT_API
#define ZMQ_BUILD_DRAFT_API 1

#include "zmq.h"
#include <vector>
#include <spdlog/spdlog.h>
#include "json.hpp"
#include "utils.h"
// #include <unistd.h>

using namespace std;
using namespace nlohmann;

namespace zmqhelper {
#define EV_MSG_DEBUG_LEN 128
#define EV_HEARTBEAT_SECONDS 30
#define MSG_HELLO "hello"
#define EV_MSG_META_PING "ping"
#define EV_MSG_META_PONG "pong"
#define EV_MSG_META_EVENT "event"

#define EV_MSG_META_TYPE_CMD "cmd"
#define EV_MSG_META_VALUE_CMD_REVESETUN "reversetun"
#define EV_MSG_META_VALUE_CMD_RESTART "restart"
#define EV_MSG_META_VALUE_CMD_UPDATE "update"
#define EV_MSG_META_VALUE_CMD_STOP "stop"


#define EV_MSG_META_TYPE_REPORT "report"
#define EV_MSG_REPORT_CATID_AVMGROFFLINE "AV_MGROFFLINE"
#define EV_MSG_REPORT_CATID_AVMODOFFLINE "AV_MODOFFLINE"
#define EV_MSG_REPORT_CATID_AVMODCONNECTED "AV_MODCONNECTED"
#define EV_MSG_REPORT_CATID_AVOPENINPUT "AV_OPENINPUT"
#define EV_MSG_REPORT_CATID_AVOPENOUTPUT "AV_OPENOUTPUT"
#define EV_MSG_REPORT_CATID_AVWRITEHEADER "AV_WRITEHEADER"
#define EV_MSG_REPORT_CATID_AVEOF "AV_EOF"
#define EV_MSG_REPORT_CATID_AVWRITEPIPE "AV_WRITEPIPE"
#define EV_MSG_REPORT_CATID_AVLOOPRESTART "AV_LOOPRESTART"
#define EV_MSG_REPORT_CATID_AVFAILEDUPLOAD "AV_FAILEDUPLOAD"

#define EV_MSG_META_VALUE_REPORT_LEVEL_INFO "info"
#define EV_MSG_META_VALUE_REPORT_LEVEL_DEBUG "debug"
#define EV_MSG_META_VALUE_REPORT_LEVEL_WARN "warn"
#define EV_MSG_META_VALUE_REPORT_LEVEL_ERROR "error"
#define EV_MSG_META_VALUE_REPORT_LEVEL_FATAL "fatal"

#define EV_MSG_META_TYPE_BROADCAST "broadcast"

#define EV_MSG_META_CONFIG "config"
#define EV_MSG_META_AVFORMATCTX "afctx"

#define EV_MSG_TYPE_AI_MOTION "ai_motion"
#define EV_MSG_TYPE_CONN_STAT "connstat"
#define EV_MSG_TYPE_SYS_STAT "sysstat"

#define EV_MSG_EVENT_MOTION_START "start"
#define EV_MSG_EVENT_MOTION_END "end"
#define EV_MSG_EVENT_CONN_CONN "connect"
#define EV_MSG_EVENT_CONN_DISCONN "disconnect"

#define EV_NUM_CACHE_PERPEER 100
#define MAX_EVENT_QUEUE_SIZE 50

//
string body2str(vector<uint8_t> body);
vector<uint8_t> data2body(char* data, int len);
vector<uint8_t> str2body(string const &str);
// proto: 1. on router [sender_id] [target_id] [body]
//        2. on dealer [sender_id] [body]
vector<vector<uint8_t> > z_recv_multiple(void *s, bool nowait=false);
// proto [sender_id(only when no identifier set in setsockopts)] [target_id] [body]
int z_send_multiple(void *s, vector<vector<uint8_t> >&body);
/// setup router
int setupRouter(void **ctx, void **s, string addr);
/// setup dealer
/// @return 0 success, otherwise failed
int setupDealer(void **ctx, void **s, string addr, string ident, int timeout = 0);
/// recv config msg:
/// @return 0 success, otherwise failed.
int recvConfigMsg(void *s, json &config, string addr, string ident);
int forkSubsystem(string devSn, string peerId, int drPort, pid_t &pid);
int z_send(void *s, string peerId, string selfId, vector<uint8_t> meta, vector<uint8_t> body);
int z_send(void *s, string peerId, string selfId, const json &meta, string body);
int z_send(void *s, string peerId, vector<uint8_t> meta, vector<uint8_t> body);
int z_send(void *s, string peerId, string meta, string body);
}

#endif