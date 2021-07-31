cmake_minimum_required(VERSION 3.10)

set(This uttt_lib)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(POSITION_INDEPENDENT_CODE ON)

project(${This} CXX)
# enable_testing()
include (CTest)

add_subdirectory(googletest)

include_directories(srcs
					.)

set (Sources
	srcs/uttt.cpp
)

set (Headers
	srcs/uttt.hpp
)

add_library(${This} SHARED ${Sources} ${Headers})

# target_sources(${This} PUBLIC ${Sources})
target_include_directories(${This} PUBLIC srcs)
target_compile_options(${This} PUBLIC -Wall -Wextra)

add_subdirectory(test)

add_executable(uttt srcs/main.cpp)

target_link_libraries(uttt PUBLIC uttt_lib)

# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fsanitize=address")

# target_compile_options(uttt PUBLIC -Wall -Wextra)
