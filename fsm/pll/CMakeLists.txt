set(LIB_NAME pll_fsm)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/fsm.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/fsm
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
    pll
)
