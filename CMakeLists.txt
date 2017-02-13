cmake_minimum_required(VERSION 2.8)
project(enigma)

add_subdirectory(src)
add_subdirectory(third_party)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -ggdb -O0 -std=c99 -D_GNU_SOURCE")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(GLIB2)
find_package(Threads REQUIRED)

set(LINK_LIBS dl ${GLIB2_LIBRARIES})




