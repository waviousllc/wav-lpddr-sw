set(DRIVER fsw_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/fsw_csp.c
    ${CMAKE_CURRENT_LIST_DIR}/fsw_ctrl.c
)

set(
    INCLUDE_DIRECTORY
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
    ${WAV_WDDR_TOP_LEVEL}/include/table
    ${WAV_WDDR_TOP_LEVEL}/include/dev
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
