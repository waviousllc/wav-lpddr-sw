set(LIB_NAME path)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/common.c
    ${CMAKE_CURRENT_LIST_DIR}/ca.c
)

set(
    INCLUDE
    ${CMAKE_SOURCE_DIR}/include/dev/
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
    pi
    gearbox
    lpde
    receiver
    sensamp
    pipeline
    driver
)
