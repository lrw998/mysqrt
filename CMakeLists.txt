cmake_minimum_required(VERSION "3.5.1")

project("mysqrt")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)# 输出可执行文件到./bin
# set(CMAKE_BINARY_DIR ./build)

add_executable("mysqrt" "mysqrt.cpp")# 指定编译一个可执行文件


