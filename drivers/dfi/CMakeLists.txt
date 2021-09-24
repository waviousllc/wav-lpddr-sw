set(DRIVER dfi_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/dfi_dp.c
    ${CMAKE_CURRENT_LIST_DIR}/dfi_fifo.c
    ${CMAKE_CURRENT_LIST_DIR}/dfi_intf.c
    ${CMAKE_CURRENT_LIST_DIR}/dfi_top.c
)

set(
    INCLUDE_DIRECTORY
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
    ${WAV_WDDR_TOP_LEVEL}/include/table
    ${WAV_WDDR_TOP_LEVEL}/include/dev
    ${WAV_WDDR_TOP_LEVEL}/include
)

add_library(
    ${DRIVER}
    STATIC
    ${SRC}
)

target_include_directories(
    ${DRIVER}
    PUBLIC
    ${INCLUDE_DIRECTORY}
)

target_link_libraries(
    ${DRIVER}
    kernel
)
