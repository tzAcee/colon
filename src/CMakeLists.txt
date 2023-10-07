cmake_minimum_required(VERSION 3.20)

set(BINARY colon)
project(${BINARY})

set(CMAKE_CXX_STANDARD 20)


set(source_files
    ./ColonParser.cpp
    ./ColonCompiler.cpp
    ./main.cpp
)

add_executable(${BINARY} ${source_files})
add_library(${BINARY}_lib STATIC ${source_files})