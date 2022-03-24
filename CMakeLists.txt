cmake_minimum_required(VERSION 3.21)
project(shogi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require (at least) it

# Create a object to hold code coverage configuration
add_library(coverage_config INTERFACE)  

option(CODE_COVERAGE "Enable code coverage reporting" OFF)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(coverage_config INTERFACE -g -O0 --coverage)
    target_link_libraries(coverage_config INTERFACE --coverage)
endif ()

add_subdirectory(src)

# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif ()
