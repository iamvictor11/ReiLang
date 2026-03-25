add_library(vvmidi STATIC)

target_include_directories(vvmidi
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(vvmidi
PRIVATE
    "src/midiOut.cpp"
    "src/msg.cpp"
)

target_link_libraries(vvmidi
PUBLIC
    winmm
)

