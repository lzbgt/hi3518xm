#ifndef _RTSP_COMMON_H
#define _RTSP_COMMON_H
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************
 *              DEFINITIONS
 ******************************************************************************/
/* this is necessary to get rid of 'useless keyword or type ..' shit */
#define __DEBUG

#define TYPES

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#ifdef __DEBUG
/* -------------------------------------------------------------------------- */
#define DBG( fmt, args...) do {\
    fprintf(stderr, "--- DEBUG:%s:%s:%d ---: ", __FILE__, __FUNCTION__, __LINE__);\
    fprintf(stderr,  fmt, ## args);} while(0)

#define ERR(fmt, args...) do {\
    fprintf(stderr, "XXX ERROR:%s:%s:%d XXX: ",__FILE__,__FUNCTION__,__LINE__);\
    fprintf(stderr, fmt,  ## args);} while (0)

#define CHECK do {\
    DBG("(*^-^)\n");} while (0)

/* debug assert (completely removed in release version) */
#define DASSERT(b,action) do {if(unlikely(!(b))){ ERR("debug assertion failure (%s)\n", #b);action;} } while (0)

/* must success (check omitted in release version) */
#define MUST(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)

/* assertion */
#define ASSERT(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)
/* sanity check wrapper (same in release and debug version) */
#define TEST(b,action) do {if(unlikely(!(b))){ ERR("unexpected (%s)\n", #b);action;} } while (0)

/* -------------------------------------------------------------------------- */
#else
/* -------------------------------------------------------------------------- */
#define DBG( fmt, args...)

#define ERR(fmt, args...) do {\
    fprintf(stderr, "XXX ERROR:%s:%s:%d XXX: ",__FILE__,__FUNCTION__,__LINE__);\
    fprintf(stderr, fmt,  ## args);} while (0)

#define CHECK 

/* debug assert (completely removed in release version) */
//#define DASSERT(b,action) 

/* must success (check omitted in release version) */
//#define MUST(b,action) do {if ((b)){}} while (0)
/* debug assert (completely removed in release version) */
#define DASSERT(b,action) do {if(unlikely(!(b))){ ERR("debug assertion failure (%s)\n", #b);action;} } while (0)

/* must success (check omitted in release version) */
#define MUST(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)


/* assertion */
#define ASSERT(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)
/* sanity check wrapper (same in release and debug version) */
#define TEST(b,action) do {if(unlikely(!(b))){ ERR("unexpected (%s)\n", #b);action;} } while (0)
/* -------------------------------------------------------------------------- */
#endif

#define SUCCESS         0
#define FAILURE         -1
#define ERR_RTSP_SHOULD_FIN -10

/* Defining infinite time */
#define FOREVER         -1

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define CLOSE(x) do{if((x)>0){close(x);(x) = 0;}}while(0)
#define FCLOSE(x) do{if((x)!=NULL){fclose(x);(x) = NULL;}}while(0)
#define ALLOC(t) (t = (typeof(t))calloc(1,sizeof(typeof(*(t)))))
#define NEW(t) ((t *)calloc(1,sizeof(t)))

#define COPY(x,y) memcpy ((x),(y),sizeof (*(y)))
//#define FREE(h) do { if(h){printf("freeing 0x%08x\n",(unsigned int)h);free(h);h=NULL;}else{DBG("freeing NULL pointer\n");}} while (0);
#define FREE(h) do { if(h){free(h);h=NULL;}} while (0);

/* common object creation macro: calloc given pointer. when failed, perform 'action' */
#define TALLOC(h,action) do {h=NULL;if(!(ALLOC(h))){ERR("No memory!\n");action;}} while (0)
    
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif

#define TRUE            1
#define FALSE           0

/* Type magic */
enum type_magic_e {
    MAGIC_NAL_QUEUE_ITEM = 0xdead0000,
    MAGIC_PACKET_QUEUE_ITEM,
    MAGIC_BUFPOOL_ELEM
};

#define CHECK_MAGIC(magic,ptr) (*(ptr) && (((unsigned int*)(*(ptr)))[0] == (magic)))

#ifndef container_of
#define container_of(type,ptr,member) ({ const typeof( ((type *)0)->member) *__mptr=(ptr); (type *) ( (char *)__mptr - offsetof(type,member));})
#endif

#endif				
