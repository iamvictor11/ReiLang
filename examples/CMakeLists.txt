function(target_setup TARGET_NAME)
    add_executable(${TARGET_NAME} "main/${TARGET_NAME}.cxx")
    set_target_properties(${TARGET_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/examples"
        CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/examples"
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${INT_DIR}/examples"
    )
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
    )
    target_link_libraries(${TARGET_NAME}
    PUBLIC
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

target_setup(01_a)

