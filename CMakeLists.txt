cmake_minimum_required(VERSION 3.20)

project(ctl VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_C_FLAGS_COVERAGE ${CMAKE_C_FLAGS} "-g --coverage")
set(CMAKE_CXX_FLAGS_COVERAGE ${CMAKE_CXX_FLAGS} "-g --coverage")

enable_testing()

add_subdirectory(contrib)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)
