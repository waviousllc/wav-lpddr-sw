set(DRIVER pll_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/driver.c
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
)

set(
    INCLUDE_DIRECTORY
    ${CMAKE_SOURCE_DIR}/include/drivers
    ${CMAKE_SOURCE_DIR}/include/dev
    ${CMAKE_SOURCE_DIR}/include
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
    vco_driver
)
