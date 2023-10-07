cmake_minimum_required(VERSION 3.10)
project(colon)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)
# Enable testing for the project

add_subdirectory(./src)
enable_testing()
add_subdirectory(./test)
