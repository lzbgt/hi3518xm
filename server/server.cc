#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <spdlog/spdlog.h>
#include <evpacket.h>

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
}packet_processor_t;

typedef struct {
    uv_tcp_t handle;
    packet_processor_t processor;
} packet_client;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_closed(uv_handle_t* handle) {
    free(handle);
}

void on_written(uv_write_t *req, int status) {
    if (status) {
        spdlog::error("Write error {}", uv_strerror(status));
    }
    free_write_req(req);
}


void debugHex(char *buf, int len){
    int i = 0;
    for(char *p = buf; i < len; i++){
        if(i % 16 == 0){
            fprintf(stderr, "\n\t");
        }
        fprintf(stderr, "%02hhX ", p[i]);
    }
    fprintf(stderr, "\n");
}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    packet_client * pclient = (packet_client *)client;
    char *data = buf->base;
    static int packetId = 0;
    spdlog::info("packetId: {}, nread: {}", ++packetId, nread);
    if (nread > 0) {
        evpacket_ptr_t pkt = (evpacket_ptr_t) data;
        if(pkt->meta.magic[0] == (char)0xBE && pkt->meta.magic[1] == (char)0xEF){
            // new packet
            pclient->processor.size = 0;
            pclient->processor.state = 1; // rcv header
        }

        while(nread > 0){
            switch(pclient->processor.state) {
                // rcv header
                case 1:{
                    if(nread + pclient->processor.size >= sizeof(evpacket_t)){
                        // whole header ready
                        auto delta = sizeof(evpacket_t) - pclient->processor.size;
                        memcpy(&pclient->processor.hdr + pclient->processor.size, data, delta);
                        pclient->processor.hdr.length = ntohl(pclient->processor.hdr.length);
                        nread -= delta;
                        spdlog::info("new packet. len {}, left: {}", pclient->processor.hdr.length, nread);
                        data += delta;
                        pclient->processor.size = 0;
                        pclient->processor.state = 2; // header ready. rcv body
                        debugHex((char*)&pclient->processor.hdr, 64);
                    }else{
                        // small header
                        memcpy(&pclient->processor.hdr + pclient->processor.size, data, nread);
                        //data += nread;
                        pclient->processor.size += nread;
                        nread = 0;
                        // state remains in rcv header
                        spdlog::info("small header. size:{}", pclient->processor.size);
                    }
                }
                break;
                case 2:{
                    // full body
                    if(nread + pclient->processor.size >= pclient->processor.hdr.length){
                        auto delta = pclient->processor.hdr.length - pclient->processor.size;
                        nread -= delta;
                        data += delta;

                        // TODO: handle body dispatch
                        spdlog::warn("TODO: full body got. left for next header: {}", nread);
                        // next 
                        pclient->processor.state = 1;
                        pclient->processor.size = 0;
                        //
                    }else{
                        //TODO: handle buf
                        pclient->processor.size += nread;
                        nread = 0;
                        spdlog::info("small body. size:{}", pclient->processor.size);
                    }

                }
                break;
                case 3:{
                    // TODO: none
                    spdlog::warn("should be here state=3");
                }
                break;
                default:{
                    spdlog::warn("should be here state=none");
                }
                break;
            }
        }
    }
    
    if (nread < 0) {
        if (nread != UV_EOF)
        {
            spdlog::error("Read error {}, closing", uv_err_name(nread));
            uv_close((uv_handle_t*) client, on_closed);
        }
        else{
            spdlog::error("read error {}", uv_err_name(nread));
        }
    }

    free(buf->base);
}

void on_connect(uv_stream_t *server, int status) {
    if (status < 0) {
        spdlog::error("New connection error {}", uv_strerror(status));
        return;
    }
    spdlog::info("client connected");
    packet_client *client = (packet_client*) malloc(sizeof(packet_client));
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

int main() {
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
