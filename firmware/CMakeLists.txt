set(
    SRC
    ${CMAKE_CURRENT_LIST_DIR}/phy_task.c
    ${CMAKE_CURRENT_LIST_DIR}/phy_api.c
)

set(
    INCLUDE
    ${WAV_WDDR_TOP_LEVEL}/include/
)

add_library(
    phy_firmware
    STATIC
    ${SRC}
)

target_compile_definitions(
    phy_firmware
    PRIVATE
    -DCONFIG_CAL_PERIODIC=${CONFIG_CAL_PERIODIC}
    -DCONFIG_CALIBRATE_PLL=${CONFIG_CALIBRATE_PLL}
    -DCONFIG_CALIBRATE_ZQCAL=${CONFIG_CALIBRATE_ZQCAL}
    -DCONFIG_CALIBRATE_SA=${CONFIG_CALIBRATE_SA}
    -DCONFIG_DRAM_TRAIN=${CONFIG_DRAM_TRAIN}
)

target_include_directories(
    phy_firmware
    PUBLIC
    ${INCLUDE}
)

target_link_libraries(
    phy_firmware
    PUBLIC
    wddr_intf
)

target_link_libraries(
    phy_firmware
    PRIVATE
    wddr
)
