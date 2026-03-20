cmake_minimum_required(VERSION 3.10)

project(rei 
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(rei main.cpp)
target_include_directories(rei
PUBLIC
    "include"
PRIVATE
    "src"
)
target_sources(rei
PRIVATE
    "src/core/base/common.cpp"
    "src/core/base/env.cpp"
    "src/core/base/string.cpp"
    "src/core/lexer/lexer.cpp"
    "src/core/lexer/token.cpp"
    "src/core/parser/parser.cpp"
    "src/core/vm.cpp"
    "src/core/vm.run.cgoto.cpp"
    "src/core/vm.run.switch.cpp"
    "src/debug/log.cpp"
    "src/stl/stl.cpp"
    "src/util/error.cpp"
    "src/util/file.cpp"
)

target_compile_options(rei
PRIVATE
    -fno-exceptions
    -fno-rtti
    $<$<CONFIG:Debug>:
        -g
        -O0
    >
    $<$<CONFIG:Release>:
        -O3
        -DNDEBUG
        -fomit-frame-pointer
        -fno-semantic-interposition
        -ffunction-sections
        -fdata-sections
        -fno-stack-protector
    >
)
target_link_options(rei
PRIVATE
    $<$<CONFIG:Release>:
        -static
        -flto
        -s
        -Wl,--gc-sections
    >
)