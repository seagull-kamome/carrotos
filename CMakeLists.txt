cmake_minimum_required(VERSION 3.13)
project(carrotos)

set(CARROT_PORT_VENDOR "vendor" CACHE INTERNAL "Board vendor name")
set(CARROT_PORT_BOARD "board" CACHE INTERNAL "Board name")
set(CARROT_PORT_CONFIG "config" CACHE INTERNAL "Board config name")


add_subdirectory(carrotlib)
add_subdirectory(vendor/${CARROT_PORT_VENDOR}/boards/${CARROT_PORT_BOARD})
add_subdirectory(kernel)


