#ifndef __RTSP_PUSHER_HPP__
#define __RTSP_PUSHER_HPP__
#include <fmt/format.h>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <httplib.h>
#include <spdlog/spdlog.h>
#include <queue>
#include <mutex>
#include <rfc.h>
#include "mime.h"
#include "common.h"

using namespace std;

#define __RTP_MAXPAYLOADSIZE (1400 - 12 -4)
const std::string RTSP_FMT_OPTIONS = "OPTIONS {} RTSP/1.0\r\nCSeq: {}\r\nUser-Agent: {}\r\n\r\n";
const std::string TEST_URI1 = "rtsp://aa:bb@evcloudsvc.ilabservice.cloud:554/pusher_test";
const std::string TEST_URI2 = "rtsp://40.73.41.176:554/pusher_test";

typedef struct DataItem {
    char *buf;
    size_t size;
    void *ud;
} *DataItemPtr;


typedef struct Notifier {
    mutex *mut;
    condition_variable *cond;
} *NotifierPtr;


typedef struct tcp_rtcp_t {
    char tcphdr[4];
    rtcp_t rtcp;
}tcp_rtcp_t;


typedef struct nal_rtp_t {
    struct {
        rtp_hdr_t header;
        signed char payload[__RTP_MAXPAYLOADSIZE];
    } packet;
    int    rtpsize;
}nal_rtp_t;

typedef struct rtp_tcp_t {
    char tcphdr[4];
    nal_rtp_t nal_rtp;
    int size;
}rtp_tcp_t;

typedef struct __time_stat_t {
    struct timeval prev_tv;
    unsigned long long avg;
    unsigned long long total_cnt;
    unsigned long long jitter_mask;
    unsigned int cnt;
    unsigned int ts_offset;
} time_stat_t;

class RtspPusher {
    public:
    typedef std::string (RtspPusher::*SDPBuilderFuncType)();
    private:
    int socket_ = -1;
    httplib::Uri targetParsedUri;
    std::string targetRawUri;
    thread th;
    int seqNum;
    std::string hostIp;
    std::string sessionId;
    NotifierPtr noti;
    queue<DataItem> *dataq;
    const std::string ua = "EVCamera";
    mime_encoded_handle spropSpsB64 = NULL;
    mime_encoded_handle spropSpsB16 = NULL;
    mime_encoded_handle spropPpsB64 = NULL;
    unsigned short rtp_seq=0;
    unsigned int rtp_timestamp;
    unsigned int ssrc=0;
    time_stat_t timeStat;
    unsigned int rtcp_octet=0;
    unsigned int rtcp_packet_cnt=0;
    int rtcp_tick=0;
    int rtcp_tick_org=0;

    int get_timestamp_offset(struct __time_stat_t *timeStat, struct timeval *p_tv)
    {
        unsigned long long  kts;

        if(timeStat->prev_tv.tv_sec == 0) {
            timeStat->prev_tv = *p_tv;
            timeStat->total_cnt = 0;
            timeStat->cnt = 0;
            timeStat->avg = 0;
            timeStat->jitter_mask = 0;
            return SUCCESS;
        }

        /* we fix time stamp offset in 5 years of running on 30fps.. */
        if(timeStat->total_cnt < 0xFFFFFFFFLLU) {


            kts = ((p_tv->tv_sec - timeStat->prev_tv.tv_sec) * 1000000 + (p_tv->tv_usec - timeStat->prev_tv.tv_usec)) * 90;

            timeStat->avg = ((timeStat->avg * timeStat->total_cnt) + kts * 1000) / (timeStat->total_cnt + 1);
            timeStat->prev_tv = *p_tv;
            timeStat->total_cnt += 1;
        }

        timeStat->jitter_mask += timeStat->avg % 1000000;
        timeStat->ts_offset = (timeStat->avg / 1000000);

        if(timeStat->jitter_mask > 1000000) {
            timeStat->ts_offset += 1;
            timeStat->jitter_mask -= 1000000;
        }

        return SUCCESS;
    }


    int __split_nal(signed char *buf, signed char **nalptr, size_t *p_len, size_t max_len)
    {
        int i;
        int start = -1;

        for(i = (*nalptr) - buf + *p_len;i<max_len-5;i++) {
            if(buf[i] == 0x00 &&
                    buf[i+1] == 0x00 &&
                    buf[i+2] == 0x00 &&
                    buf[i+3] == 0x01) {
                if(start == -1){
                    i += 4;
                    start = i;
                } else {
                    *nalptr = &(buf[start]);
                    while(buf[i-1] == 0) i--;
                    *p_len = i - start;
                    return 0;
                }
            }
        }

        if(start == -1) {
            /* malformed NAL */
            return -1;
        }

        *nalptr = &(buf[start]);
        *p_len = max_len + 2 - start;

        return 0;
    }

    int probe_sprop(signed char *buf, size_t len)
    {
        signed char *nalptr;
        size_t single_len;
        mime_encoded_handle base64 = NULL;
        mime_encoded_handle base16 = NULL;
        unsigned int pt;
        
        /* check SPS is set */
        if(!spropSpsB64){ 
            nalptr = buf;
            single_len = 0;
            while (__split_nal(buf,&nalptr,&single_len,len) == SUCCESS) {
                pt = nalptr[0] & 0x1F;
                if(pt == H264_NAL_TYPE_SPS) {
                    ASSERT(base64 = mime_base64_create((char *)&(nalptr[0]),single_len), return FAILURE);
                    ASSERT(base16 = mime_base16_create((char *)&(nalptr[1]),3), return FAILURE);

                    DASSERT(base16->base == 16, return FAILURE);
                    DASSERT(base64->base == 64, return FAILURE);

                    if(spropSpsB64) {
                        DBG("sps is set by another thread?\n");
                        mime_encoded_delete(base64);
                    }
                    spropSpsB64 = base64;

                    if(spropSpsB16) {
                        DBG("sps is set by another thread?\n");
                        mime_encoded_delete(base16);
                    }
                    spropSpsB16 = base16;
                }

            }

            base64 = NULL;
            base16 = NULL;
        }

        /* check PPS is set */
        if(!spropPpsB64){
            nalptr = buf;
            single_len = 0;
            while (__split_nal(buf,&nalptr,&single_len,len) == SUCCESS) {
                pt = nalptr[0] & 0x1F;

                if(pt == H264_NAL_TYPE_PPS) {
                    ASSERT(single_len >= 4, return FAILURE);
                    ASSERT(base64 = mime_base64_create((char *)&(nalptr[0]),single_len), return FAILURE);

                    DASSERT(base64->base == 64, return FAILURE);
                    if(spropPpsB64) {
                        DBG("pps is set by another thread?\n");
                        mime_encoded_delete(base64);
                    }
                    spropPpsB64 = base64;
                }
            }
            base64 = NULL;
        }

        return SUCCESS;
    }

    int connect(std::string host, std::string port){
        int rv = 0;
        struct addrinfo hints{};
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;       // IPv4
        hints.ai_socktype = SOCK_STREAM; // TCP
        hints.ai_flags = AI_NUMERICSERV; // port passed as as numeric value
        hints.ai_protocol = 0;

        addrinfo *addrinfo_result;
        rv = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &addrinfo_result);
        if (rv != 0)
        {
            spdlog::error("::getaddrinfo failed: {}", gai_strerror(rv));
            return rv;
        }

        int last_errno = 0;
        for (auto *rp = addrinfo_result; rp != nullptr; rp = rp->ai_next)
        {
            socket_ = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (socket_ == -1)
            {
                last_errno = errno;
                continue;
            }
            rv = ::connect(socket_, rp->ai_addr, rp->ai_addrlen);
            if (rv == 0)
            {
                auto addr = (struct sockaddr_in *)rp->ai_addr; 
                hostIp = string(inet_ntoa((struct in_addr)addr->sin_addr));
                break;
            }
            else
            {
                last_errno = errno;
                ::close(socket_);
                socket_ = -1;
                rv = -1;
            }
        }
        ::freeaddrinfo(addrinfo_result);
        return rv;
    }

    size_t send(const char *data, size_t n_bytes)
    {
        size_t bytes_sent = 0;
        while (bytes_sent < n_bytes)
        {
            const int send_flags = 0;
            auto write_result = ::send(socket_, data + bytes_sent, n_bytes - bytes_sent, send_flags);

            if (write_result == 0)
            {
                break;
            }
            bytes_sent += static_cast<size_t>(write_result);
        }

        return bytes_sent;
    }

    int srecv(char *ptr, size_t size/*, time_t sec, time_t usec*/) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(socket_, &fds);

        timeval tv;
        tv.tv_sec = static_cast<long>(5);
        tv.tv_usec = static_cast<long>(0);

        if(select(static_cast<int>(socket_ + 1), &fds, nullptr, nullptr, &tv) > 0)
        {
            return ::recv(socket_, ptr, static_cast<int>(size), 0);
        }else {
            return -1;
        }
    }

    static inline unsigned long long __get_random_byte(unsigned *ctx)
    {
        return (unsigned long long)(rand_r(ctx) % 256);
    }

    static inline unsigned long long __get_random_llu(unsigned *ctx)
    {
        return 0
            | __get_random_byte(ctx)
            | (__get_random_byte(ctx)) << 8 
            | (__get_random_byte(ctx)) << 16 
            | (__get_random_byte(ctx)) << 24
            | (__get_random_byte(ctx)) << 32
            | (__get_random_byte(ctx)) << 40
            | (__get_random_byte(ctx)) << 48
            | (__get_random_byte(ctx)) << 56;
    }

    int _sendRtp(void *v){
        int send_bytes;
        rtp_tcp_t *rtp_tcp = (rtp_tcp_t *) v;
        nal_rtp_t *rtp = &(rtp_tcp->nal_rtp);

        rtp_tcp->size = rtp->rtpsize + 4;

         // rtp over tcp 4 bytes header
        rtp_tcp->tcphdr[0] = '$';
        rtp_tcp->tcphdr[1] = 0;
        rtp_tcp->tcphdr[2] = (char)((rtp->rtpsize & 0xFF00)>>8);
        rtp_tcp->tcphdr[3] = (char)(rtp->rtpsize&0xFF);

        rtp->packet.header.seq = htons(rtp_seq);
        rtp->packet.header.ts = htonl(rtp_timestamp);
        rtp->packet.header.ssrc = htonl(ssrc);
        rtp_seq += 1;

        send_bytes = ::send(socket_,v, rtp_tcp->size, 0);
        if(send_bytes == rtp_tcp->size) {
            rtcp_packet_cnt += 1;
            rtcp_octet += rtp_tcp->size;
            return 0;
        }

        if(send_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){
            ERR("EAGAIN\n");
            return -1;
        }
        
        ERR("send:%d:%s\n",send_bytes,strerror(errno));
        return -1;
    }

    int sendRtp(signed char *nalbuf, size_t nalsize)
    {
        rtp_tcp_t rtp_tcp;
        nal_rtp_t *rtp = &rtp_tcp.nal_rtp;
        signed char *nalptr = nalbuf;

        unsigned int nri = nalptr[0] & 0x60;
        unsigned int pt  = nalptr[0] & 0x1F;
        rtp_hdr_t *p_header = &(rtp->packet.header);
        signed char *payload = rtp->packet.payload;
        int rv = 0;

        p_header->version = 2;
        p_header->p = 0;
        p_header->x = 0;
        p_header->cc = 0;
        p_header->pt = 96 & 0x7F;

        if(nalsize <= __RTP_MAXPAYLOADSIZE){
            /* single packet */
            /* SPS, PPS, SEI is not marked */
            if(pt != 7 && pt != 8 && pt != 6) { 
                p_header->m = 1;
            } else {
                p_header->m = 0;
            }

            memcpy(payload, nalptr, nalsize);
            rtp->rtpsize = nalsize + sizeof(rtp_hdr_t);
            // TODO: send
            rv = _sendRtp(&rtp_tcp);
            if(rv < 0) {
                spdlog::error("failed to send one packet rtp");
                return rv;
            }
            
        }  else  {
            //spdlog::warn("got big frame! {}", nalsize);
            nalptr += 1;
            nalsize -= 1;

            payload[0] = 28;
            payload[0] |= nri;
            payload[1] = pt;
            payload[1] |= 1 << 7;

            /* send fragmented nal */
            while(nalsize > __RTP_MAXPAYLOADSIZE - 2){
                p_header->m = 0;
                memcpy(&(payload[2]), nalptr, __RTP_MAXPAYLOADSIZE - 2);

                rtp->rtpsize = sizeof(rtp_hdr_t) + __RTP_MAXPAYLOADSIZE;

                nalptr += __RTP_MAXPAYLOADSIZE - 2;
                nalsize -= __RTP_MAXPAYLOADSIZE - 2;

                // ASSERT(__rtp_send_h264(&rtp,trans_list) == SUCCESS, return FAILURE);
                // TODO: send
                rv = _sendRtp(&rtp_tcp);
                if(rv < 0) {
                    spdlog::error("failed to send full packet rtp");
                    return rv;
                }
                /* intended xor. blame vim :( */
                payload[1] &= 0xFF ^ (1<<7); 
            }

            /* send trailing nal */
            p_header->m = 1;

            payload[1] |= 1 << 6;

            /* intended xor. blame vim :( */
            payload[1] &= 0xFF ^ (1<<7);

            rtp->rtpsize = nalsize + sizeof(rtp_hdr_t);

            memcpy(&(payload[2]), nalptr, nalsize);
            rv = _sendRtp(&rtp_tcp);

            // ASSERT(__rtp_send_h264(&rtp, trans_list) == SUCCESS, return FAILURE);
            // TODO: send

        }

        return rv;
    }

    int sendRtpFullPacket(char * nalbuf, size_t size) {
        int ret = 0;
        unsigned int len = size + 12;
        // send 4 bytes magic first;

        unsigned int nri = nalbuf[0] & 0x60;
        unsigned int pt  = nalbuf[0] & 0x1F;
        char magic[4+12];
        magic[0] = '$';
        magic[1] = 0;
        magic[2] = (char)((len& 0xFF00)>>8);
        magic[3] = (char)(len&0xFF);
        // 12 bytes header
        rtp_hdr_t *p_header = (rtp_hdr_t *)(magic+4);
        p_header->version = 2;
        p_header->p = 0;
        p_header->x = 0;
        p_header->cc = 0;
        p_header->pt = 96 & 0x7F;
        p_header->seq = htons(rtp_seq);
        p_header->ts = htonl(rtp_timestamp);
        p_header->ssrc = htonl(ssrc);
        if(pt != 7 && pt != 8 && pt != 6) { 
            p_header->m = 1;
        } else {
            p_header->m = 0;
        }

        ret = ::send(socket_, magic, 16, 0);
        if(ret <0) {
            spdlog::error("failed to send header");
            exit(1);
        }

        ret = ::send(socket_, nalbuf, size, 0);
        if(ret != size) {
            spdlog::error("failed to send body, {}, {}", ret, size);
            exit(1);
        }

        return 0;
    }

    string sendRtcpSr()
    {
        
        unsigned int ts_h; 
        unsigned int ts_l; 
        int send_bytes;
        struct sockaddr_in to_addr;
        tcp_rtcp_t tcp_rtcp;
        rtcp_t *rtcp = &tcp_rtcp.rtcp;
        tcp_rtcp.tcphdr[0] = 0x24;
        tcp_rtcp.tcphdr[1] = 0; // channel
        tcp_rtcp.tcphdr[2] = (char)((28 & 0xFF00)>>8);
        tcp_rtcp.tcphdr[3] = (char)(28 & 0xFF);

        struct timeval tv;
        ::gettimeofday(&tv,NULL);
        ts_h = (unsigned int)tv.tv_sec + 2208988800u;
        ts_l = (tv.tv_usec << 12) + (tv.tv_usec << 8) - ((tv.tv_usec * 3650) >> 6);
        spdlog::info("timeofday sec:{}, usec:{}", tv.tv_sec, tv.tv_usec);

        rtcp->common.version = 2; //
        rtcp->common.length = htons(6);
        rtcp->common.p = 0;
        rtcp->common.count = 0;
        rtcp->common.pt = RTCP_SR;
        rtcp->r.sr.ssrc = htonl(ssrc);
        rtcp->r.sr.ntp_sec = htonl(ts_h);
        rtcp->r.sr.ntp_frac = htonl(ts_l);
        rtcp->r.sr.rtp_ts = htonl(rtp_timestamp);
        rtcp->r.sr.psent = htonl(rtcp_packet_cnt);
        rtcp->r.sr.osent = htonl(rtcp_octet);
        ASSERT((send_bytes = ::send(socket_, &(tcp_rtcp), 32, 0)) == 32, ({
                    ERR("send :%, %",send_bytes,strerror(errno));
                    return "error";}));

        rtcp_packet_cnt = 0;
        rtcp_octet = 0;
        rtcp_tick = rtcp_tick_org;

        return "";
    }
    

    std::string buildOptsString(){
        std::string opts = fmt::format(RTSP_FMT_OPTIONS, targetRawUri, (seqNum = 1), ua);
        return opts;
    }

    std::string buildAnnounceString()
    {
        std::string sdp = fmt::format(
            "v=0\r\n"
            "o=- {} 0 IN IP4 {}\r\n"
            "s={}\r\n"
            "c=IN IP4 {}\r\n"
            "t=0 0\r\n"
            "a=tool:libavformat 58.29.100\r\n"
            //"a=control:*\r\n"
            "m=video 0 RTP/AVP 96\r\n"
            "a=rtpmap:96 H264/90000\r\n"
            "a=fmtp:96 packetization-mode=1;"
            " profile-level-id={};"
            " sprop-parameter-sets={},{};\r\n" 
            "a=control:streamid={}\r\n",
            0, "127.0.0.1",ua, "0.0.0.0",spropSpsB16->result, spropSpsB64->result, spropPpsB64->result, 0); //(long)std::time(NULL)

        std::string ann = fmt::format(
            "ANNOUNCE {} RTSP/1.0\r\n"
            "Content-Type: application/sdp\r\n"
            "CSeq: {}\r\n"
            "User-Agent: {}\r\n"
            "Session: {}\r\n"
            "Content-Length: {}\r\n\r\n",
            targetRawUri, 
            ++seqNum,
            ua,
            sessionId,
            sdp.length()
            );
        return ann + sdp;
    }

    std::string buildSetupString(){
        std::string setup = fmt::format(
            "SETUP {}/streamid={} RTSP/1.0\r\n"
            "Transport: RTP/AVP/TCP;unicast;mode=record;interleaved={}-{}\r\n"
            "CSeq: {}\r\n"
            "User-Agent: {}\r\n"
            "Session: {}\r\n"
            "\r\n",
            targetRawUri, 
            0,
            0,
            1,
            ++seqNum,
            ua,
            sessionId
            );
        return setup;
    }

    std::string buildStartString(){
        std::string record = fmt::format(
            "RECORD {} RTSP/1.0\r\n"
            "Range: npt=0.000-\r\n"
            "CSeq: {}\r\n"
            "User-Agent: {}\r\n"
            "Session: {}\r\n"
            "\r\n",
            targetRawUri, 
            ++seqNum,
            ua, 
            sessionId);

        return record;
    }

    std::string buildStopString(){
        std::string td = fmt::format(
        "TEARDOWN {} RTSP/1.0\r\n"
        "CSeq: {}\r\n"
        "User-Agent: {}\r\n"
        "Session: {}\r\n\r\n",
        targetRawUri,
        ++seqNum,
        ua,
        sessionId);

        return td;
    }

    int init_session(RtspPusher::SDPBuilderFuncType *builders){
        // send OPTIONS
        int rv = 0;
        if(targetParsedUri.Port.empty()){
           targetParsedUri.Port = "554";
        }
        rv = connect(targetParsedUri.Host, targetParsedUri.Port);
        if(rv != 0) {
            spdlog::error("failed to connect host {}:{}", targetParsedUri.Host, targetParsedUri.Port);
            return rv;
        }

        char line[1024] = {0};
        this->seqNum = 1;
        for(int i = 0; i <4; i++){
            std::string s;
            size_t sz;
            s = (this->*builders[i])();
            sz = send(s.data(), s.size());
            spdlog::info("\n\nsend: {},\n{}", s.size(), s);
            memset(line, 0, sizeof(line));
            sz = srecv(line, 1023);
            if(sz < 0){
                spdlog::error("failed to connect to server");
                return sz;
            }

            spdlog::info("\nreads: {},\n{}", sz, line);
            if(this->sessionId.empty()){
                size_t pos = 0, opos = 0, tpos = 0;
                std::string sline(line);
                for(pos=sline.find("\r\n");pos!=string::npos && (pos+2)!=sline.size();opos = pos+2, pos=sline.find("\r\n", opos)){
                    std::string l = sline.substr(opos, pos-opos);    
                    spdlog::info("line :{}", l);
                    if((tpos=l.find("Session: ")) != string::npos){
                        this->sessionId = l.substr(9);
                        spdlog::info(   "sess string: {}", this->sessionId);
                    }
                }
            }
        }
        sendRtcpSr();

        return 0;
    }

    public:
    RtspPusher(std::string url, NotifierPtr noti ,queue<DataItem> *dataq): targetRawUri(url), noti(noti), dataq(dataq){
        targetParsedUri = httplib::Uri::Parse(url);
        rtp_seq = rand_r((unsigned *)this);
        rtp_timestamp = (unsigned int)(__get_random_llu((unsigned int *)this));
        ssrc = (unsigned int)(__get_random_llu((unsigned int *)this));
        spdlog::info("u:{}, p:{}, h:{}, p:{}, {}, {} ", targetParsedUri.User, targetParsedUri.Password, targetParsedUri.Host, targetParsedUri.Port, targetParsedUri.Path, targetParsedUri.QueryString);
    }
    ~RtspPusher(){
    }

    int start(thread &th) {
        int rv = 0;

        SDPBuilderFuncType builders[] = {
            &RtspPusher::buildOptsString,
            &RtspPusher::buildAnnounceString,
            &RtspPusher::buildSetupString,
            &RtspPusher::buildStartString,
        };

        th = thread([this, builders](){
            int ret;
            static bool sessionInited = false;
            unsigned long long frameCnt = 0;
            while(1){
                unique_lock<mutex> lk(*this->noti->mut);
                this->noti->cond->wait(lk, [this] {return !this->dataq->empty();});
                auto elem = this->dataq->front();
                this->dataq->pop();
                MaQueVideoEncFrameInfo_s *pMem = (MaQueVideoEncFrameInfo_s *)elem.ud;
                if(frameCnt %5 == 0){
                    frameCnt++;
                    spdlog::info("got {} frame to send {}", frameCnt, pMem->nDataLen);
                }
                
                // probe sps pps
                if(!sessionInited &&(this->spropPpsB64 == NULL || this->spropSpsB16 == NULL||this-> spropSpsB64 == NULL)){
                     ret = this->probe_sprop((signed char*)elem.buf, elem.size);
                     if(ret != 0) {
                         spdlog::error("failed to probe sprops");
                     }
                }else{
                    if(!sessionInited) {
                        if(pMem->eSubType == MAQUE_FRAME_SUBTYPE_I) {
                            spdlog::info("sps16 {}, sps64 {}, pps64 {}", this->spropSpsB16->result, this->spropSpsB64->result, this->spropPpsB64->result);
                            ret = init_session((RtspPusher::SDPBuilderFuncType*) builders);
                            if(ret != 0){
                                spdlog::error("failed to init_session");
                                exit(1);
                            }
                            sessionInited = true;
                        } 
                    }
                }

                if(sessionInited){
                    struct timeval tv;
                    ::gettimeofday(&tv,NULL);
                    get_timestamp_offset(&timeStat, &tv);
                    if(this->sendRtp((char *)elem.buf, elem.size) < 0){
                        spdlog::error("socket error: failed to send");
                        //sessionInited = false;
                        exit(1);
                    }
                }
                // get data
                //send(elem.buf, elem.size);
                MaQue_Demo_Mem_release(pMem->handleMem);
            }
        });
        if(th.joinable()){
            th.detach();
            return 0;
        }else{
            return -5;
        }
    }
};

#endif

// int test(){
//     //RtspPusher pusher1(TEST_URI1);
//     RtspPusher pusher2(TEST_URI2);
//     pusher2.start();
// }

// int main(){
//     test();
// }