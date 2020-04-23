
#ifndef __EVPACKET_H__
#define __EVPACKET_H__

#define TERMINAL_SN_SIZE 10
typedef enum evpacket_type_t {
    EV_PACKET_TYPE_NONE,
    EV_PACKET_TYPE_H264,
    EV_PACKET_TYPE_H265,
    EV_PACKET_TYPE_JPEG,
    EV_PACKET_TYPE_AUDIO,
    EV_PACKET_TYPE_NUM
} evpacket_type_t;

typedef enum evframe_type_t {
    EVFRAME_NONE,
    EVFRAME_I,
    EVFRAME_P,
    EVFRAME_B,
    EVFRAME_NUM
}evframe_type_t;

typedef struct evresolution_t {
    uint16_t height;
    uint16_t width;
}evresolution_t;

typedef struct evpacket_meta_t {
    /// 0xBE, 0xEF
    char magic[2];
    uint64_t packet_id;
    char sn[TERMINAL_SN_SIZE];
    uint16_t crc;
}*evpacket_meta_ptr_t;

typedef struct evpacket_video_t {
    uint16_t packet_type;
    uint16_t frame_type;
    evresolution_t res;
    uint64_t ts;
}*evpacket_video_ptr_t;

typedef struct evpacket_t {
    evpacket_meta_t meta;
    evpacket_video_t vpara;
    uint32_t length;
} *evpacket_ptr_t;


static inline unsigned short crc16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}


#endif