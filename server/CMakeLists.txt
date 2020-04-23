cmake_minimum_required (VERSION 3.0)
project (server)
add_definitions(-std=c++1z -g -fpermissive)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)

if("${VENDOR}" STREQUAL "")
    set(VENDOR "${CMAKE_SOURCE_DIR}/../vendor")
    set(SHAREDINC "${CMAKE_SOURCE_DIR}/../include")
endif()

set(COMM_INC_DIR ${VENDOR}/shared/include ${VENDOR}/x64/include ${SHAREDINC})
set(COMMON_LIB_DIR ${VENDOR}/shared/lib ${VENDOR}/x64/lib)
# list(APPEND COMMON_LIBS  avformat avdevice avcodec swscale avfilter avutil swresample lzma x264 fmt uv pthread dl m z)
list(APPEND COMMON_LIBS avformat avcodec swscale avutil swresample lzma x264 fmt uv pthread dl m z)
include_directories(${CMAKE_SOURCE_DIR} ${PROJECT_SOURCE_DIR} ${COMM_INC_DIR})
link_directories(${COMMON_LIB_DIR})

#add_library(mime STATIC mime.c)
#add_library(util STATIC utils.cpp)

add_executable(server server.cc)
target_link_libraries(server PUBLIC ${COMMON_LIBS})
