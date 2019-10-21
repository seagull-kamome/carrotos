cmake_minimum_required(VERSION 3.12)
project(carrot_arch_cortex-m3)

add_library(carrot_arch_cortex-m3
  src/timer.c)
target_include_directories(carrot_arch_cortex-m3
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(carrot_arch_cortex-m3 PUBLIC carrot_port carrot_compiler)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION include)

