set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)

# set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb")

find_program(CARROT_CC arm-none-eabi-gcc
  PATHS $<CARROT_GCC_PATH>
  PATH_SUFFIXES bin
  NO_DEFAULT_PATH
  )
set(CARROT_CC "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_C_COMPILER ${CARROT_CC} CACHE FILEPATH "C compiler")
set(CMAKE_C_COMPILER_FORCED TRUE)
#set(CMAKE_C_COMPILER_WORKS TRUE)

get_filename_component(CARROT_CC_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component(CMAKE_FIND_ROOT_PATH "${CARROT_CC_DIR}" DIRECTORY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_library(carrot_compiler INTERFACE)
#target_compile_features(carrot_compiler INTERFACE c_std_11)
target_compile_options(carrot_compiler INTERFACE
  $<$<CONFIG:Release>:-O3>
  $<$<CONFIG:Debug>:-O0 -g>
  $<$<CONFIG:RelWithDebugInfo>:-O3 -g>
  -std=gnu11 -Wall -fdata-sections -ffunction-sections
  )
target_compile_definitions(carrot_compiler INTERFACE
  CARROT_CONFIG_COMPILER=CARROT_COMPILER_GCC
  $<$<NOT:$<CONFIG:Debug>>:NODEBUG>
  )
target_link_libraries(carrot_compiler INTERFACE carrot_compiler_port_gcc)

