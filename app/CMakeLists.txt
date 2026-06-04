add_executable(rei_vm "main.cpp")

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
    target_link_libraries(${TARGET_NAME}
    PUBLIC
        rei
    )
    set_property(TARGET ${TARGET_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    target_compile_options(${TARGET_NAME}
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
    target_link_options(${TARGET_NAME}
    PRIVATE
        -static
        $<$<CONFIG:Release>:
            -s
            -Wl,--gc-sections
        >
    )
endfunction()

setup_target(rei_vm)
