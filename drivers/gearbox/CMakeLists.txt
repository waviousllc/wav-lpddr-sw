set(DRIVER gearbox_driver)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/rx_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/tx_driver.c
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
)
