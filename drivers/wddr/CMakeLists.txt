FILE(
    GLOB_RECURSE
    WDDR_DRIVER_SOURCES
    ${WAV_WDDR_TOP_LEVEL}/drivers/*/*.c
)

set(
    INCLUDE_DIRECTORY
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
    ${WAV_WDDR_TOP_LEVEL}/include/dev
    ${WAV_WDDR_TOP_LEVEL}/include/table
    ${WAV_WDDR_TOP_LEVEL}/include
)

add_library(
    wddr_driver
    STATIC
    ${WDDR_DRIVER_SOURCES}
)

target_include_directories(
    wddr_driver
    PUBLIC
    ${INCLUDE_DIRECTORY}
)

target_link_libraries(
    wddr_driver
    messenger_driver
    kernel
)
