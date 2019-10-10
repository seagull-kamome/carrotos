cmake_minimum_required(VERSION 3.13)
project(kernel C)

add_library(kernel
  src/timer.c
  )
target_include_directories(kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(kernel PUBLIC c_std_11)
target_link_libraries(kernel PUBLIC
  carrotlib kernel_board_port kernel_compiler_port)



