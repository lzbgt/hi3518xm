cmake_minimum_required (VERSION 3.0)
project (evcamera)
add_definitions(-std=c++1z -fpermissive -fPIC -ffunction-sections -fdata-sections -Wl,-gc-sections)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

if("${VENDOR}" STREQUAL "")
    set(VENDOR "${CMAKE_SOURCE_DIR}/../vendor")
    set(SHAREDINCS "${CMAKE_SOURCE_DIR}/../include")
endif()


#set_source_files_properties(main.cc PROPERTIES COMPILE_FLAGS -g)

set(COMM_INC_DIR ${VENDOR}/shared/include ${VENDOR}/hi3518/include ${SHAREDINCS})
set(COMMON_LIB_DIR ${VENDOR}/shared/lib ${VENDOR}/hi3518/lib)
list(APPEND COMMON_LIBS fmt m dl pthread XmMaQue securec XmSns_50H20AI uv)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${COMM_INC_DIR})
link_directories(${COMMON_LIB_DIR} /root/xiongmai/arm-himix100-linux/target/lib/)

#add_library(mime STATIC mime.c)
add_library(ntp STATIC ntp.cc)
add_library(tcp_client STATIC raw_tcp.cc)
add_library(smart STATIC smart.cc)
add_library(motion STATIC motion.cc)
#add_library(util STATIC utils.cpp)

add_executable(evcamera main.cc)
target_link_libraries(evcamera PUBLIC ntp tcp_client smart motion ${COMMON_LIBS})

add_executable(ntp_client ntp_main.cc)
target_link_libraries(ntp_client PUBLIC ntp)
