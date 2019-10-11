cmake_minimum_required(VERSION 3.13)
include(./tools/cmake/toolchain/${CARROT_PORT_COMPILER}.cmake)

project(carrotos C)

set(CARROT_PORT_VENDOR "vendor" CACHE STRING "Board vendor name")
set(CARROT_PORT_BOARD "board" CACHE STRING "Board name")
set(CARROT_PORT_CONFIG "config" CACHE STRING "Board config name")


add_subdirectory(carrotlib)
add_subdirectory(vendor/${CARROT_PORT_VENDOR}/boards/${CARROT_PORT_BOARD})
add_subdirectory(kernel)


