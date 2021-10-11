FILE(
    GLOB_RECURSE
    WDDR_DEV_SOURCES
    ${WAV_WDDR_TOP_LEVEL}/dev/*/*.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/dev/
)

add_library(
    wddr
    STATIC
    ${WDDR_DEV_SOURCES}
)

target_include_directories(
    wddr
    PUBLIC
    ${INCLUDE}
)

add_library(
    wddr_ext
    INTERFACE
)

target_link_libraries(
    wddr
    wddr_driver
    wddr_ext
)
