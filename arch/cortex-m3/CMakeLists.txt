cmake_minimum_required(VERSION 3.12)
project(carrot_arch_cortex-m3)

add_library(carrot_arch_cortex-m3 INTERFACE)
target_include_directories(carrot_arch_cortex-m3
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(carrot_arch_cortex-m3 INTERFACE carrot_compiler)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION include)

