#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <spdlog/spdlog.h>
#include <evpacket.h>
#include <sys/time.h>

extern "C" {
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}

using namespace std;

#define DEFAULT_PORT 7123
#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

typedef struct {
    uv_tcp_t *handle;
    evpacket_t hdr;
    char * buf;
    unsigned int size;
    int state; // 0 - unkown; 1 - receiving header; 2 - header ready, receiving body; 3 - body ready, processing
    AVFormatContext *pAvCtx;
    uint64_t pktCnt;
} packet_processor_t;

typedef struct {
    uv_tcp_t handle;
    packet_processor_t processor;
} packet_client;

void free_write_req(uv_write_t *req)
{
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_closed(uv_handle_t* handle)
{
    packet_client * pclient = (packet_client *)handle;
    spdlog::error("closing client");
    if(pclient->processor.buf){
        free(pclient->processor.buf);
    }
    avformat_free_context(pclient->processor.pAvCtx);
    free(handle);
}

void on_written(uv_write_t *req, int status)
{
    if (status) {
        spdlog::error("Write error {}", uv_strerror(status));
    }
    free_write_req(req);
}


void debugHex(char *buf, int len)
{
    int i = 0;
    for(char *p = buf; i < len; i++) {
        if(i % 16 == 0) {
            fprintf(stderr, "\n\t");
        }
        fprintf(stderr, "%02hhX ", p[i]);
    }
    fprintf(stderr, "\n");
}


// char *url = "rtsp://evcloudsvc.ilabservice.cloud/test_pusher";
AVFormatContext* rtsp_init(string rtsp_url, int codec, int height, int width)
{
    int ret;
    int codec_idx = 0;
    struct timeval tv, tv2;
    //int bCheckBufWrong;
    int bps = 0;
    ::gettimeofday(&tv, nullptr);
    AVFormatContext *pAVFormatRemux = nullptr;
    AVDictionary *pOptsRemux = nullptr;

    if(av_dict_set(&pOptsRemux, "rtsp_transport", "tcp", 0) < 0) {
        spdlog::error("failed set output pOptsRemux");
        return nullptr;
    }
    // timeout in microseconds
    av_dict_set_int(&pOptsRemux, "stimeout", (int64_t)(1000* 1000 * 1), 0);
    ret = avformat_alloc_output_context2(&pAVFormatRemux, nullptr, "rtsp", rtsp_url.c_str());
    AVStream *out_stream = avformat_new_stream(pAVFormatRemux, nullptr);
    if (!out_stream) {
        spdlog::error("Failed allocating output stream\n");
        return nullptr;
    }

    AVCodecParameters *out_codecpar = out_stream->codecpar;
    memset(out_codecpar, 0, sizeof(AVCodecParameters));
    out_codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    out_codecpar->codec_id = AV_CODEC_ID_H264;
    out_codecpar->bit_rate = 400000;
    out_codecpar->width = width;
    out_codecpar->height = height;
    out_codecpar->codec_tag = 0;
    out_codecpar->format = AV_PIX_FMT_YUV420P;
    avio_open2(&pAVFormatRemux->pb, rtsp_url.c_str(), AVIO_FLAG_WRITE, nullptr, &pOptsRemux);
    ret = avformat_write_header(pAVFormatRemux, &pOptsRemux);
    av_dict_free(&pOptsRemux);

    return pAVFormatRemux;
}

static volatile int64_t ptsInc = 0;
int write_packet(AVFormatContext *ctx, char *data, int len){
  int ret = 0;
  AVPacket pkt;
  AVStream *out_stream = ctx->streams[0];
	av_init_packet(&pkt);
	// isI = isIdrFrame1((uint8_t*)data, len);
	// pkt.flags |= isI ? AV_PKT_FLAG_KEY : 0;
    memset(&pkt, 0 , sizeof(pkt));
	pkt.stream_index = 0;
	pkt.data = (uint8_t*)data;
	pkt.size = len;
	//wait I frame
	// if (waitI) {
	// 	if (0 == (pkt.flags & AV_PKT_FLAG_KEY))
	// 		return;
	// 	else
	// 		waitI = 0;	
	// }
	
	AVRational time_base;
	time_base.den = 60;
	time_base.num = 1;
    pkt.dts = av_rescale_q_rnd(++ptsInc, out_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
    pkt.pts = av_rescale_q((++ptsInc) * 2, time_base, out_stream->time_base);
	pkt.duration = 0; //av_rescale_q(pkt.duration, out_stream->time_base, out_stream->time_base);
	pkt.pos = -1;

	/* copy packet (remuxing In the example)*/
	//pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
	//pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
	//pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
	//pkt.pos = -1;

	ret = av_write_frame(ctx, &pkt);
	if (ret < 0) {
		fprintf(stderr, "Error muxing packet\n");
	}

    ::free(data);
	av_packet_unref(&pkt);

  return ret;
}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    packet_client * pclient = (packet_client *)client;
    char *data = buf->base;
    static int packetId = 0;
    spdlog::debug("packetId: {}, nread: {}", ++packetId, nread);
    if (nread > 0) {
        evpacket_ptr_t pkt = (evpacket_ptr_t) data;
        if(nread >= 2 && pkt->meta.magic[0] == (char)0xBE && pkt->meta.magic[1] == (char)0xEF) {
            // new packet
            pclient->processor.size = 0;
            pclient->processor.state = 1; // rcv header
            
        }

        while(pclient->processor.state != 0 && nread > 0) {
            switch(pclient->processor.state) {
            // rcv header
            case 1: {
                if(nread + pclient->processor.size >= sizeof(evpacket_t)) {
                    // whole header ready
                    auto delta = sizeof(evpacket_t) - pclient->processor.size;
                    memcpy(&pclient->processor.hdr + pclient->processor.size, data, delta);
                    pclient->processor.hdr.length = ntohl(pclient->processor.hdr.length);
                    pclient->processor.hdr.vpara.res.height = ntohs(pclient->processor.hdr.vpara.res.height);
                    pclient->processor.hdr.vpara.res.width = ntohs(pclient->processor.hdr.vpara.res.width);
                    spdlog::debug("new packet. len {}, left: {}, width:{}, height:{}", pclient->processor.hdr.length, nread,
                                 pclient->processor.hdr.vpara.res.width,pclient->processor.hdr.vpara.res.height);
                    //debugHex((char*)&pclient->processor.hdr, 64);

                    pclient->processor.buf = (char *)malloc(pclient->processor.hdr.length);
                    if(pclient->processor.pAvCtx == nullptr) {
                        spdlog::info("device sn: {}", pclient->processor.hdr.meta.sn);
                        pclient->processor.pAvCtx = rtsp_init(string("rtsp://evcloudsvc.ilabservice.cloud/") + string(pclient->processor.hdr.meta.sn), AV_CODEC_ID_H264, pclient->processor.hdr.vpara.res.height, pclient->processor.hdr.vpara.res.width);
                    }

                    nread -= delta;
                    data += delta;
                    pclient->processor.size = 0;
                    pclient->processor.state = 2; // header ready. rcv body
                }
                else {
                    // small header
                    memcpy(&pclient->processor.hdr + pclient->processor.size, data, nread);
                    //data += nread;
                    pclient->processor.size += nread;
                    nread = 0;
                    // state remains in rcv header
                    spdlog::debug("small header. size:{}", pclient->processor.size);
                }
            }
            break;
            case 2: {
                // full body
                if(nread + pclient->processor.size >= pclient->processor.hdr.length) {
                    auto delta = pclient->processor.hdr.length - pclient->processor.size;
                    memcpy(pclient->processor.buf + pclient->processor.size, data, delta);
                    nread -= delta;
                    data += delta;
                    // TODO: handle body dispatch
                    spdlog::debug("TODO: full body got. left for next header: {}", nread);
                    auto ret = write_packet(pclient->processor.pAvCtx, pclient->processor.buf, pclient->processor.hdr.length);
                    if(ret < 0) {
                        spdlog::error("failed to send packet");
                        exit(1);
                    }

                    // next
                    pclient->processor.state = 1;
                    pclient->processor.size = 0;
                    //
                }
                else {
                    //TODO: handle buf
                    memcpy(pclient->processor.buf + pclient->processor.size, data, nread);
                    pclient->processor.size += nread;
                    nread = 0;
                    spdlog::debug("small body. size:{}", pclient->processor.size);
                }

            }
            break;
            case 3: {
                // TODO: none
                spdlog::warn("should be here state=3");
            }
            break;
            default: {
                spdlog::warn("should be here state=none");
                // TODO: close and free resources
                uv_close((uv_handle_t*) client, on_closed);
            }
            break;
            }
        }
    }

    if (nread < 0) {
        if (nread != UV_EOF) {
            spdlog::error("Read error {}, closing", uv_err_name(nread));
            uv_close((uv_handle_t*) client, on_closed);
        }
        else {
            spdlog::error("read error {}", uv_err_name(nread));
        }
    }

    free(buf->base);
}

void on_connect(uv_stream_t *server, int status)
{
    if (status < 0) {
        spdlog::error("New connection error {}", uv_strerror(status));
        return;
    }
    spdlog::info("client connected");
    packet_client *client = (packet_client*) ::calloc(sizeof(packet_client), 1);
    client->processor.handle = &client->handle;
    uv_tcp_init(loop, (uv_tcp_t *)client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, on_read);
    }
    else {
        spdlog::error("failed to accept");
        uv_close((uv_handle_t*) client, on_closed);
    }
}

int main()
{
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    spdlog::info("sizeof pkt header {}, sizeof tv {}", sizeof(evpacket_t), sizeof(timeval));
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_connect);
    if (r) {
        spdlog::error("Listen error {}", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
