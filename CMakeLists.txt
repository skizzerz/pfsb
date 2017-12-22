cmake_minimum_required(VERSION 3.2)
project(PFSB)

# Set up general compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-Wall -Wextra -pedantic)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif()
	add_compile_options(/W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-Wall -Wextra -pedantic)
endif()

message("${CMAKE_CXX_FLAGS}")

add_subdirectory(src bin) # execute src/CMakeLists.txt, putting its output in bin/
