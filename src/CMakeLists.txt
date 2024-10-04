cmake_minimum_required(VERSION 3.10)
project(leafpy_lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(API_DIR ${SRC_DIR}/api)
set(INC_DIR ${SRC_DIR}/inc)
set(CORE_DIR ${SRC_DIR}/core ${SRC_DIR}/core/leafpy ${SRC_DIR}/core/testcore)
set(LIB_DIR ${SRC_DIR}/lib)

include_directories(${API_DIR} ${INC_DIR} ${CORE_DIR} ${LIB_DIR})

file(GLOB_RECURSE SRCS "*.c")
add_library(leafpy_lib ${SRCS})