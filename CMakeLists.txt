if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project("WAV-LPDDR-SW" C ASM)
    cmake_minimum_required(VERSION 3.13)

    # Set the output folder where your program will be created
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Make sure everything is built for correct architecture
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

    # Build configuration
    include(${CMAKE_CURRENT_LIST_DIR}/CMake.in)

    # Build versioning
    set(VERSION_UPDATE_FROM_GIT TRUE)
    find_package(BuildInfo REQUIRED)

    add_compile_options(
        -O2
        -g
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
        -fdata-sections
        -ffunction-sections
        -std=c11
    )
endif()

################################################################################
##                           CONFIGURATIONS
################################################################################
# Set flag for skipping PLL calibration at boot
set(CONFIG_CALIBRATE_PLL true CACHE BOOL "Flag to indicate if PLL calibration is performed at boot.")
message("PLL CAL:       ${CONFIG_CALIBRATE_PLL}")

# Set flag for skipping ZQCAL calibration at boot
set(CONFIG_CALIBRATE_ZQCAL true CACHE BOOL "Flag to indicate if ZQCAL calibration is performed at boot.")
message("ZQCAL CAL:     ${CONFIG_CALIBRATE_ZQCAL}")

# Set flag for skipping Sense Amp calibration at boot
set(CONFIG_CALIBRATE_SA true CACHE BOOL "Flag to indicate if SA calibration is performed at boot.")
message("SA CAL:        ${CONFIG_CALIBRATE_SA}")

# Set flag for skipping DRAM training at boot
set(CONFIG_DRAM_TRAIN true CACHE BOOL "Flag to indicate if DRAM training is performed at boot.")
message("DRAM_TRAIN:    ${CONFIG_DRAM_TRAIN}")
################################################################################
##                        SOURCE DIRECTORIES
################################################################################/
# Build subdirectories
add_subdirectory(${WAV_RTOS_TOP_LEVEL})
add_subdirectory(drivers)
add_subdirectory(dev)
add_subdirectory(fsm)
add_subdirectory(app)
