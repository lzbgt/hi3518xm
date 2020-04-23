#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include "raw_tcp.h"

int raw_connect(std::string host, std::string port, int *socket_, int recv_timeout, int send_timeout){
    int rv = 0;
    struct addrinfo hints{};
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_NUMERICSERV; // port passed as as numeric value
    hints.ai_protocol = 0;

    spdlog::info("raw_connect");

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
        *socket_ = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (*socket_ == -1)
        {
            last_errno = errno;
            continue;
        }
        rv = ::connect(*socket_, rp->ai_addr, rp->ai_addrlen);
        if (rv == 0)
        {
            auto addr = (struct sockaddr_in *)rp->ai_addr; 
            // //auto hostIp = string(inet_ntoa((struct in_addr)addr->sin_addr));
            // rv = fcntl(*socket_, F_SETFL, fcntl(*socket_, F_GETFL, 0) | O_NONBLOCK);
            // if (rv == -1){
            //     spdlog::error("error calling fcntl");
            // }
            break;
        }
        else
        {
            last_errno = errno;
            ::close(*socket_);
            *socket_ = -1;
            rv = -1;
        }
    }
    struct timeval timeout;      
    timeout.tv_sec = recv_timeout;
    timeout.tv_usec = 0;

    if (setsockopt (*socket_, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
        spdlog::error("setsockopt SO_RCVTIMEO failed");
        return -1;
    }

    timeout.tv_sec = send_timeout;

    if (setsockopt (*socket_, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        spdlog::error("setsockopt SO_SNDTIMEO failed");
        return -1;
    }

    ::freeaddrinfo(addrinfo_result);
    return rv;
}