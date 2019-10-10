cmake_minimum_required(VERSION 3.13)
project(port_st_nucleo-f03rb)


##############################################################################

add_library(compiler_port INTERFACE)
target_compile_options(compiler_port INTERFACE
  $<$<CONFIG:Release>:-O3>
  $<$<CONFIG:Debug>:-O0 -g>
  $<$<CONFIG:RelWithDebugInfo>:-O3 -g>
  -Wall -fdata-sections -ffunction-sections
  )
target_compile_definitions(compiler_port INTERFACE
  CARROT_CONFIG_COMPILER=CARROT_COMPILER_GCC
  $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
  )
add_library(kernel_compiler_port ALIAS compiler_port)
add_library(carrotlib_compiler_port ALIAS compiler_port)


##############################################################################

add_library(kernel_board_port INTERFACE)
target_include_directories(kernel_board_port
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(kernel_board_port INTERFACE carrotlib)



