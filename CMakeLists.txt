cmake_minimum_required(VERSION 3.10)

set(This ft_containers)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 98)
set(POSITION_INDEPENDENT_CODE ON)

project(${This} CXX)
# enable_testing()
include (CTest)

add_subdirectory(googletest)

include_directories(srcs
					.)

# set(Headers
# 	/Users/frthierr/studlife/ft_containers/build/test/Memd/mem.h
# 	srcs/ft_containers.hpp
# 	srcs/iterators/vector_iterator.hpp
# 	srcs/iterators/list_iterator.hpp
# 	srcs/templates/reverse_iterator.hpp
# 	srcs/templates/constBool.hpp
# 	srcs/templates/enable_if.hpp
# 	srcs/templates/list_node.hpp
# 	srcs/templates/is_integral.hpp
# 	srcs/containers/vector.hpp
# 	srcs/containers/list.hpp
# 	src/includes/pair.hpp
# )

add_library(${This} INTERFACE)

target_sources(${This} INTERFACE)
target_include_directories(${This} INTERFACE Headers)
target_compile_options(${This} INTERFACE -Wall -Wextra -Werror)

# add_subdirectory(test)