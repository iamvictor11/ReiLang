cmake_minimum_required(VERSION 3.10)

project(luna 
    VERSION 1.0.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
else()
    add_compile_options(-O2 -Wall -Wextra)
endif()

add_executable(luna main.c)
target_include_directories(luna
PUBLIC
    "include"
PRIVATE
    "src"
)
target_sources(luna
PRIVATE
    "src/base.c"
    "src/lexer.c"
    "src/pool.c"
    "src/vm.c"
)
