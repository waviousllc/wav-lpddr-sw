set(LIB_NAME  dfi)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/buffer.c
    ${CMAKE_CURRENT_LIST_DIR}/command.c
    ${CMAKE_CURRENT_LIST_DIR}/device.c
    ${CMAKE_CURRENT_LIST_DIR}/packet.c
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
    dfi_driver
)
