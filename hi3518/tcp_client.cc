#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <spdlog/spdlog.h>
#include "common.h"

extern XM_S32 MaQue_Demo_Mem_release(XM_HANDLE handle);

static void on_close(uv_handle_t* handle);
static void on_connect(uv_connect_t* req, int status);
void on_write(uv_write_t* req, int status);
static CallBackArg *_callBackArgs = nullptr;
static uv_loop_t *loop = nullptr;
static void (*_callback)(void *arg) = nullptr;

static void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    *buf = uv_buf_init((char*)malloc(size), size);
}

static void on_close(uv_handle_t* handle)
{
    spdlog::info("closed.");
}

void on_write(uv_write_t* req, int status)
{
    if (status) {
        spdlog::error("uv_write error");
        return;
    }
    uv_buf_t * bufs = req->bufs;
    MaQueVideoEncFrameInfo_s * pMem = (MaQueVideoEncFrameInfo_s *)((bufs[0].base) - (char *)((MaQueVideoEncFrameInfo_s *)NULL)->pData);
    MaQue_Demo_Mem_release(pMem->handleMem);

    free(req);
    //uv_close((uv_handle_t*)req->handle, on_close);
}

static void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
{
    spdlog::info("on_read {0:p}",(void*)tcp);
    if(nread >= 0) {
        //spdlog::info("read: {}", tcp->data);
        spdlog::info("read: {0:p}", (void*)(buf->base));
    }
    else {
        //we got an EOF
        uv_close((uv_handle_t*)tcp, on_close);
    }

    //cargo-culted
    free(buf->base);
}

static void write_data(uv_stream_t* stream, char *data, size_t size)
{
    uv_buf_t buffer[] = {
        {.base = data, .len = size}
    };
    uv_write_t *req = (uv_write_t *)malloc(sizeof(uv_write_t));
    uv_write(req, stream, buffer, 1, on_write);
}

static void on_connect(uv_connect_t* connection, int status)
{
    if (status < 0) {
        spdlog::error("failed to connect");
        return;
    }
    spdlog::info("connn");
    spdlog::info("connected. {0:d}", status);
    uv_thread_t sender;
    uv_stream_t* stream = connection->handle;
    free(connection);
    _callBackArgs->ud = (void*)stream;
    uv_thread_create(&sender, _callback, (void*)_callBackArgs);

    uv_thread_join(&sender);

    // write_data(stream, "echo  world!", 12);
    // uv_read_start(stream, alloc_cb, on_read);
}

static void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res)
{
    if (status < 0) {
        spdlog::error("getaddrinfo callback error {}", uv_err_name(status));
        return;
    }

    char addr[17] = {'\0'};
    uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, 16);
    spdlog::error("{}", addr);

    uv_connect_t *connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));
    uv_tcp_t *socket = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, socket);
    uv_tcp_connect(connect_req, socket, (const struct sockaddr*) res->ai_addr, on_connect);
    uv_freeaddrinfo(res);
}

int start_trans(const char* host, const char* port, void (*callback)(void *arg), CallBackArg *callbackArgs, uv_run_mode mode)
{
    _callBackArgs = callbackArgs;
    _callback = callback;
    loop = uv_default_loop();
    struct addrinfo hints;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo_t resolver;
    spdlog::error("starting tcp client");
    int r = uv_getaddrinfo(loop, &resolver, on_resolved, "192.168.55.104", "7000", &hints);
    if (r) {
        spdlog::error("getaddrinfo call error {}", uv_err_name(r));
        return 1;
    }

    uv_run(loop, mode/*UV_RUN_DEFAULT*/);
}