set(DRIVER cmn_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/cmn_clk.c
    ${CMAKE_CURRENT_LIST_DIR}/cmn_ibias.c
    ${CMAKE_CURRENT_LIST_DIR}/cmn_pmon.c
    ${CMAKE_CURRENT_LIST_DIR}/cmn_rstn.c
    ${CMAKE_CURRENT_LIST_DIR}/cmn_vref.c
    ${CMAKE_CURRENT_LIST_DIR}/cmn_zqcal.c
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
