set(LIB_NAME wddr)
set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/device.c
)

set(
    INCLUDE
    ${CMAKE_SOURCE_DIR}/include/dev/
    ${CMAKE_SOURCE_DIR}/include/fsm/
)

add_library(
    ${LIB_NAME}
    STATIC
    ${SRC}
)

if (CONFIG_CALIBRATE_PLL)
    message("PLL Calibration enabled")
    target_compile_definitions(${LIB_NAME} PRIVATE -DCONFIG_CALIBRATE_PLL)
endif (CONFIG_CALIBRATE_PLL)

if (CONFIG_CALIBRATE_ZQCAL)
    message("ZQCAL Calibration enabled")
    target_compile_definitions(${LIB_NAME} PRIVATE -DCONFIG_CALIBRATE_ZQCAL)
endif (CONFIG_CALIBRATE_ZQCAL)

if (CONFIG_CALIBRATE_SA)
    message("SA Calibration enabled")
    target_compile_definitions(${LIB_NAME} PRIVATE -DCONFIG_CALIBRATE_SA)
endif (CONFIG_CALIBRATE_SA)

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
