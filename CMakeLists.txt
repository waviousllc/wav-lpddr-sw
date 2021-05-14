project("WAV-LPDDR-SW" C ASM)
cmake_minimum_required(VERSION 3.13)

# Set the output folder where your program will be created
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Make sure everything is built for correct architecture
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

################################################################################
##                           CONFIGURATIONS
################################################################################
# Set flag for skipping PLL calibration at boot
set(CONFIG_CALIBRATE_PLL true CACHE BOOL "Flag to indicate if PLL calibration is performed at boot.")

# Set flag for skipping ZQCAL calibration at boot
set(CONFIG_CALIBRATE_ZQCAL true CACHE BOOL "Flag to indicate if ZQCAL calibration is performed at boot.")

# Set flag for skipping Sense Amp calibration at boot
set(CONFIG_CALIBRATE_SA true CACHE BOOL "Flag to indicate if SA calibration is performed at boot.")

set(RTOS_HEAP_ALLOC_SCHEME "4")
include(${CMAKE_CURRENT_LIST_DIR}/rtos/CMake.in)
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

################################################################################
##                        SOURCE DIRECTORIES
################################################################################/
# Build subdirectories
add_subdirectory(${WAV_RTOS_TOP_LEVEL})
add_subdirectory(drivers)
add_subdirectory(dev)
add_subdirectory(fsm)
add_subdirectory(app)
