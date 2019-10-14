cmake_minimum_required(VERSION 3.12)
project(port_st_nucleo-f03rb C)

add_subdirectory(../../../../arch/cortex-m3 arch)
add_library(carrot_arch ALIAS carrot_arch_cortex-m3)

##############################################################################

add_library(carrot_compiler_port_gcc INTERFACE)
target_compile_options(carrot_compiler_port_gcc INTERFACE
  -march=armv7-m -mtune=cortex-m3 -mthumb)




##############################################################################

add_library(carrot_port INTERFACE)
target_include_directories(carrot_port
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(carrot_port INTERFACE carrotlib)



