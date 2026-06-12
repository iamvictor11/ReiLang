add_executable(lexer "main.lexer.c")

function(setup_target TARGET_NAME)
    target_compile_definitions(${TARGET_NAME}
    PRIVATE
        TEST_BUILD_DEBUG=$<IF:$<CONFIG:Debug>,1,0>
        TEST_BUILD_RELEASE=$<IF:$<CONFIG:Release>,1,0>
    )
    target_include_directories(${TARGET_NAME}
    PUBLIC
        "include"
        "src"
    )
    target_sources(${TARGET_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/kua.c"
    )
    target_link_libraries(${TARGET_NAME}
    PUBLIC
        rei
    )
    set_property(TARGET ${TARGET_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    target_compile_options(${TARGET_NAME}
    PRIVATE
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
    target_link_options(${TARGET_NAME}
    PRIVATE
        -static
        $<$<CONFIG:Release>:
            -s
            -Wl,--gc-sections
        >
    )
endfunction()

setup_target(lexer)
