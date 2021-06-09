set(DRIVER pipeline_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/egress_ana_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/egress_dig_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/ddr_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/qdr_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/rt_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/sdr_driver.c
)

set(
    INCLUDE_DIRECTORY
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
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
