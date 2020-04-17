#ifndef __EV_COMMON_H__
#define __EV_COMMON_H__
#include <queue>
#include <mutex>
// #include <uv.h>

typedef struct DataItem {
    char *buf;
    uint32_t size;
    void *ud;
} *DataItemPtr;


typedef struct Notifier {
    std::mutex *mut;
    std::condition_variable *cond;
} *NotifierPtr;


typedef struct {
    ::FILE * recFD;
    NotifierPtr noti;
    std::queue<DataItem> *dataq;
    void *ud;
} CallBackArg;

// typedef struct{
//     MaQueVideoEncFrameInfo_s *pMem;
//     uv_stream_t* pStream;
// } UserDefined;

// void on_write(uv_write_t* req, int status);
// int start_trans(const char* host, const char* port, void (*callback)(void *arg), CallBackArg *callbackArgs, uv_run_mode mode);

#endif