add_library(rei STATIC)

function(target_setup TARGET_SNAME TARGET_LNAME)
target_compile_definitions(${TARGET_SNAME}
PRIVATE
    ${TARGET_LNAME}_BUILD_DEBUG=$<IF:$<CONFIG:Debug>,1,0>
    ${TARGET_LNAME}_BUILD_RELEASE=$<IF:$<CONFIG:Release>,1,0>
)
target_include_directories(${TARGET_SNAME}
PUBLIC
    "include"
PRIVATE
    "src"
)
target_sources(${TARGET_SNAME}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/asm/rei_vm.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/rei_lexer.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler/rei_token.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/env/rei_environment.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rei_allocation_callbacks.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rei.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rei_error.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/rei_log.cxx"
)
target_link_libraries(${TARGET_SNAME}
PUBLIC
)
endfunction()

target_setup(rei REI)
