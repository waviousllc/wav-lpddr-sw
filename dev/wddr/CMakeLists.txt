set(LIB_NAME wddr)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/device.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/dev/
    ${WAV_WDDR_TOP_LEVEL}/include/fsm/
)

add_library(
    ${LIB_NAME}
    STATIC
    ${SRC}
)

target_compile_definitions(
    ${LIB_NAME}
    PRIVATE
    -DCONFIG_CALIBRATE_PLL=${CONFIG_CALIBRATE_PLL}
    -DCONFIG_CALIBRATE_ZQCAL=${CONFIG_CALIBRATE_ZQCAL}
    -DCONFIG_CALIBRATE_SA=${CONFIG_CALIBRATE_SA}
    -DCONFIG_DRAM_TRAIN=${CONFIG_DRAM_TRAIN}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${INCLUDE}
)

add_library(
    wddr_ext
    INTERFACE
)

target_link_libraries(
    ${LIB_NAME}
    wddr_driver
    path
    dfi
    pll
    dram
    wddr_ext
    fsw
)
