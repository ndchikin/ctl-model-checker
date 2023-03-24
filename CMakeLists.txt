cmake_minimum_required(VERSION 3.20)

project(ctl VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(contrib)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)
