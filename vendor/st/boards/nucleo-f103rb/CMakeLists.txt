cmake_minimum_required(VERSION 3.13)
project(port_st_nucleo-f03rb C)


##############################################################################

add_library(carrot_compiler_port_gcc INTERFACE)
target_compile_options(carrot_compiler_port_gcc INTERFACE
  -mcpu=cortex-m3 -mthumb)




##############################################################################

add_library(kernel_board_port INTERFACE)
target_include_directories(kernel_board_port
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(kernel_board_port INTERFACE carrotlib)



