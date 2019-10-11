cmake_minimum_required(VERSION 3.13)
project(carrotlib C)

add_library(carrotlib
  src/time.c
  )
target_include_directories(carrotlib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(carrotlib
  PRIVATE carrot_compiler)


