cmake_minimum_required(VERSION 3.15)
project(soaring LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add Duktape
add_subdirectory(duktape)

# Add Catch2
add_subdirectory(external/catch2)

# Core library
add_library(js_core
    src/core.cpp
    src/js_engine.cpp
    src/host_api.cpp
)
target_include_directories(js_core PUBLIC src duktape)

# Unit test executable
enable_testing()
add_subdirectory(tests)