set(DRIVER ca_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/ca_bscan.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_dp.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_egress.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_gearbox.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_lpde.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_pi.c
    ${CMAKE_CURRENT_LIST_DIR}/ca_top.c
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
