set(LIB_NAME wddr)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/device.c
    ${CMAKE_CURRENT_LIST_DIR}/train/common.c
    ${CMAKE_CURRENT_LIST_DIR}/train/command_bus.c
    ${CMAKE_CURRENT_LIST_DIR}/train/read.c
    ${CMAKE_CURRENT_LIST_DIR}/train/write.c
    ${CMAKE_CURRENT_LIST_DIR}/train/eye_algo.c
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

target_link_libraries(
    ${LIB_NAME}
    wddr_driver
    pll_fsm
    fs_fsm
    dfi_master_fsm
    dfi_update_fsm
    path
    dfi
    pll
    dram
)
