cmake_minimum_required(VERSION 3.13)
project(carrotlib C)

add_library(carrotlib
  src/time.c
  )
target_include_directories(carrotlib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(carrotlib PRIVATE
  $<$<CONFIG:Release>:-O3>
  $<$<CONFIG:Debug>:-O0 -g>
  $<$<CONFIG:RelWithDebugInfo>:-O3 -g>
  -Wall -fdata-sections -ffunction-sections
  )
target_compile_definitions(carrotlib PRIVATE
  CARROT_CONFIG_COMPILER=CARROT_COMPILER_GCC
  $<$<NOT:$<CONFIG:Debug>>:NODEBUG>
  )
target_compile_features(carrotlib PUBLIC c_std_11)


