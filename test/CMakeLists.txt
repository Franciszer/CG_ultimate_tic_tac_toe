cmake_minimum_required(VERSION 3.10)

set(This gtest_utt)
set(CMAKE_CXX_STANDARD 17)

# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fsanitize=address")
# SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -fsanitize=address")

set(Sources
	srcs/maps.cpp
)

add_executable(${This} ${Sources})

# target_include_directories(${This} PUBLIC srcs)

target_link_libraries(${This} PUBLIC
	gtest_main
	utt)

add_test(
	NAME ${This}
	COMMAND ${This}
)
