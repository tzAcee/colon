set(BINARY colon_test)

set(CMAKE_CXX_STANDARD 20)

set(header_files
    ./doctest.h
    ./ColonLexerTest.h
    ./ColonParserTest.h
    ./ColonCompilerTest.h
)

set(source_files
    ./main.cpp
)

add_executable(colon_test ${source_files} ${header_files})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC colon_lib)