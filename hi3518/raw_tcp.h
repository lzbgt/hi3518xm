#ifndef __RAW_SOCKET_H__
#define __RAW_SOCKET_H__
#include <fmt/format.h>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <spdlog/spdlog.h>
#include <queue>
#include <mutex>
#include "common.h"

using namespace std;
int raw_connect(std::string host, std::string port, int *socket_);
#endif