# Minimal CMakeLists.txt for manually added Duktape
cmake_minimum_required(VERSION 3.15)
project(duktape C)

add_library(duktape STATIC duktape.c)
target_include_directories(duktape PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
