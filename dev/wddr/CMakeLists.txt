set(LIB_NAME wddr)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/device.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/dev/
    ${WAV_WDDR_TOP_LEVEL}/include/fsm/
)

add_library(
    ${LIB_NAME}
    STATIC
    ${SRC}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${INCLUDE}
)

add_library(
    wddr_ext
    INTERFACE
)

target_link_libraries(
    ${LIB_NAME}
    wddr_driver
    dfi
    pll
    dram
    wddr_ext
    fsw
    cmn
    ctrl
    channel
)
