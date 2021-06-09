set(LIB_NAME path)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/common.c
    ${CMAKE_CURRENT_LIST_DIR}/ca.c
    ${CMAKE_CURRENT_LIST_DIR}/dq.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/dev/
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

target_link_libraries(
    ${LIB_NAME}
    vref
    zqcal
    ibias
    pmon
    pi_driver
    gearbox_driver
    lpde_driver
    receiver
    sensamp
    pipeline
    driver
)
