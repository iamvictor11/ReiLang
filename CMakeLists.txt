cmake_minimum_required(VERSION 3.10)

project(luna 
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
else()
    add_compile_options(-O2 -Wall -Wextra)
endif()

add_executable(luna main.cpp)
target_include_directories(luna
PUBLIC
    "include"
PRIVATE
    "src"
)
target_sources(luna
PRIVATE
    "src/core/base/common.cpp"
    "src/core/base/string.cpp"
    "src/core/lexer/lexer.cpp"
    "src/core/lexer/token.cpp"
    "src/core/parser/parser.RD.cpp"
    "src/core/parser/evaluator.cpp"
    "src/core/parser/printer.cpp"
    "src/core/vm.cpp"
    "src/debug/log.cpp"
    "src/util/error.cpp"
    "src/util/file.cpp"
    "src/kua.cpp"
)
