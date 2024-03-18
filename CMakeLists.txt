cmake_minimum_required(VERSION 3.27)
project(mpu9250 LANGUAGES C VERSION 0.0.0)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SOURCE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)
set(SOURCE_LIST)
foreach(SOURCE_DIR ${SOURCE_DIRS})
    file(GLOB_RECURSE RECURSE_SOURCES ${SOURCE_DIR}/*.c ${SOURCE_DIR}/*.h)
    list(APPEND SOURCE_LIST ${RECURSE_SOURCES})
endforeach()

include(cppcheck.cmake)
include(doxygen.cmake)
include(uncrustify.cmake)

include_directories(include)
set(SRCS src/Mpu9250.c)
add_library(mpu9250 ${SRCS})
add_dependencies(mpu9250 uncrustify)
