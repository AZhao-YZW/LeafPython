cmake_minimum_required(VERSION 3.10)
project(${demo_name}_demo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(DEMO_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${CMAKE_SOURCE_DIR}/../../src)
set(API_DIR ${SRC_DIR}/api)

include_directories(${DEMO_DIR} ${API_DIR})

add_subdirectory(${SRC_DIR} leafpy_lib)
add_executable(${demo_name}_demo ${DEMO_DIR}/main.c leafpy_lib)