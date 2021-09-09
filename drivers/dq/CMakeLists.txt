set(DRIVER dq_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/dq_bscan.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_dp.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_egress.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_gearbox.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_lpde.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_pi.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_receiver.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_sa.c
    ${CMAKE_CURRENT_LIST_DIR}/dq_top.c
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
