add_library(rei STATIC)

function(setup_target TARGET_SNAME TARGET_LNAME)
target_compile_definitions(${TARGET_SNAME}
PRIVATE
    ${TARGET_LNAME}_BUILD_DEBUG=$<IF:$<CONFIG:Debug>,1,0>
    ${TARGET_LNAME}_BUILD_RELEASE=$<IF:$<CONFIG:Release>,1,0>
)
target_include_directories(${TARGET_SNAME}
PUBLIC
    "include"
    "src"
)
target_compile_options(${TARGET_SNAME}
PRIVATE
    -fno-exceptions
    -fno-rtti
    $<$<CONFIG:Debug>:
        -O0
        -g
    >
    $<$<CONFIG:Release>:
        -O3
        -DNDEBUG
        -fomit-frame-pointer
        -fno-semantic-interposition
        -ffunction-sections
        -fdata-sections
    >
)
target_link_options(${TARGET_SNAME}
PRIVATE
    $<$<CONFIG:Release>:
        -s
        -Wl,--gc-sections
    >
)
endfunction()

setup_target(rei REI)
target_sources(rei
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/chunk.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/debug/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/type/value.cpp"
)
