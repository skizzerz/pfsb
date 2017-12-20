cmake_minimum_required(VERSION 3.2)
project(Program VERSION 0.2.1)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in" "${PROJECT_SOURCE_DIR}/config.hpp")
add_executable(pfsb main.cpp pfMon.cpp)