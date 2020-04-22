#include "ntp.h"
#include "time.h"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[])
{
    time_t stm;
    if(getNtpTime(&stm) >=0) {
        if(stime(&stm) < 0) {
            spdlog::error("failed to set system time");
        }
        else {
            spdlog::info("ntp got time");
        }
    }
    return 0;
}