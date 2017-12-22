cmake_minimum_required(VERSION 3.2)
project(main VERSION 0.2.1)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in" "${PROJECT_SOURCE_DIR}/config.hpp")
find_package(Boost 1.61 REQUIRED COMPONENTS filesystem program_options)

add_executable(main main.cpp pfMon.cpp pfMeta.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 14)
target_link_libraries(main PRIVATE ${Boost_LIBRARIES})
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
