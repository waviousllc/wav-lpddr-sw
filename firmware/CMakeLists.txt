set(LIB_NAME phy_firmware)

set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/phy_task.c
    ${CMAKE_CURRENT_LIST_DIR}/phy_api.c
)

add_library(
    ${LIB_NAME}
    STATIC
    ${SRC}
)

target_compile_options(
    ${LIB_NAME}
    PRIVATE
    -Wno-missing-field-initializers
)

target_compile_definitions(
    ${LIB_NAME}
    PRIVATE
    -DCONFIG_CAL_PERIODIC=${CONFIG_CAL_PERIODIC}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${INCLUDE_DIRECTORY}
)

target_link_libraries(
    ${LIB_NAME}
    wddr
    clk_driver
)
