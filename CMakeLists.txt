cmake_minimum_required(VERSION 3.15)

project(Boost.Application VERSION 0.4.12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(ADDRESS_SANITIZER "Enable address sanitizer" OFF)
option(BUILD_EXAMPLES "Build example code" OFF)

find_package(Boost REQUIRED COMPONENTS thread unit_test_framework)

add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors -Wunused-result)

if (ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)

install(DIRECTORY include/boost TYPE INCLUDE)

enable_testing()

add_subdirectory(test)

if (BUILD_EXAMPLES)
    add_subdirectory(example)
endif ()
