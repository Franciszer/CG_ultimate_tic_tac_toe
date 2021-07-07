cmake_minimum_required(VERSION 3.10)

set(This utt)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(POSITION_INDEPENDENT_CODE ON)

project(${This} CXX)
# enable_testing()
include (CTest)

add_subdirectory(googletest)

# include_directories(srcs
# 					.)

set (Sources
	srcs/main.cpp
)

add_library(${This} STATIC ${Sources})

# target_sources(${This} PUBLIC ${Sources})
target_include_directories(${This} PUBLIC srcs)
target_compile_options(${This} PUBLIC -Wall -Wextra -Werror)

add_subdirectory(test)