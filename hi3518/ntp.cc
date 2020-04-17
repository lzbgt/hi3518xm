#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <spdlog/spdlog.h>

#include "common.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

int getNtpTime(time_t * txTm)
{
    int socket_ = 0, n = 0, rv = 0; // Socket file descriptor and the n return result from writing/reading from the socket.
    int portno = 123; // NTP UDP port number.
    const char* host_name = "cn.pool.ntp.org"; // NTP server host-name.

    // Structure that defines the 48 byte NTP packet protocol.
    typedef struct {

        uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
        // li.   Two bits.   Leap indicator.
        // vn.   Three bits. Version number of the protocol.
        // mode. Three bits. Client will pick mode 3 for client.

        uint8_t stratum;         // Eight bits. Stratum level of the local clock.
        uint8_t poll;            // Eight bits. Maximum interval between successive messages.
        uint8_t precision;       // Eight bits. Precision of the local clock.

        uint32_t rootDelay;      // 32 bits. Total round trip delay time.
        uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
        uint32_t refId;          // 32 bits. Reference clock identifier.

        uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
        uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

        uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
        uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

        uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
        uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

        uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
        uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

    } ntp_packet;              // Total: 384 bits or 48 bytes.

    // Create and zero out the packet. All 48 bytes worth.

    ntp_packet packet;
    memset( &packet, 0, sizeof( ntp_packet ) );
    // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

    *( ( char * ) &packet + 0 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

    struct addrinfo hints{};
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM; // TCP
    hints.ai_flags = AI_NUMERICSERV; // port passed as as numeric value
    hints.ai_protocol = 0;

    spdlog::info("get ntp host info");
    addrinfo *addrinfo_result;
    rv = ::getaddrinfo(host_name, std::to_string(123).c_str(), &hints, &addrinfo_result);
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
            auto hostIp = inet_ntoa((struct in_addr)addr->sin_addr);
            spdlog::info("ntp host {} has ip {}", host_name, hostIp);
            // rv = fcntl(*socket_, F_SETFL, fcntl(*socket_, F_GETFL, 0) | O_NONBLOCK);
            // if (rv == -1){
            //     spdlog::error("error calling fcntl");
            // }
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
    if(rv < 0){
        return rv;
    }

    // Send it the NTP packet it wants. If n == -1, it failed.

    n = write( socket_, ( char* ) &packet, sizeof( ntp_packet ) );

    if ( n < 0 ) {
        spdlog::error("ERROR writing to socket" );
        return -1;
    }


    // Wait and receive the packet back from the server. If n == -1, it failed.

    n = read( socket_, ( char* ) &packet, sizeof( ntp_packet ) );

    if ( n < 0 ) {
        spdlog::error("ERROR reading from socket" );
        return -1;
    }


    // These two fields contain the time-stamp seconds as the packet left the NTP server.
    // The number of seconds correspond to the seconds passed since 1900.
    // ntohl() converts the bit/byte order from the network's to host's "endianness".

    packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
    packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

    // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
    // Subtract 70 years worth of seconds from the seconds since 1900.
    // This leaves the seconds since the UNIX epoch of 1970.
    // (1900)------------------(1970)**************************************(Time Packet Left the Server)

    *txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );

    // Print the time we got from the server, accounting for local timezone and conversion from UTC time.

    spdlog::info( "Time: {}", (int)ctime( ( const time_t* ) &txTm ) );

    return 0;
}