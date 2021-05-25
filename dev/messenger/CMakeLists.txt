set(LIB_NAME messenger)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/device.c
)

set(
    INCLUDE
    ${CMAKE_SOURCE_DIR}/include/dev/
    ${CMAKE_SOURCE_DIR}/include/drivers/
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
    messenger_driver
)
