cmake_minimum_required(VERSION 3.12)
project(carrotlib C)

add_library(carrotlib
  src/c_startup.c
  src/time.c
  src/timer.c
  src/bitops.c
  src/spinlock.c
  )
target_include_directories(carrotlib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(carrotlib
  PRIVATE carrot_compiler carrot_port carrot_arch)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include)

