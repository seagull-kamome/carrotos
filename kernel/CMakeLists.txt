cmake_minimum_required(VERSION 3.13)
project(kernel C)

add_library(kernel
  src/timer.c
  )
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(kernel PRIVATE
  carrot_compiler
  carrotlib carrotlib
  kernel_board_port)


