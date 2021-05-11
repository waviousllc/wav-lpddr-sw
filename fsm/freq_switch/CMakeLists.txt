set(LIB_NAME fs_fsm)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
)

set(
    INCLUDE
    ${CMAKE_SOURCE_DIR}/include/fsm
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
    kernel
    pll_fsm
)
