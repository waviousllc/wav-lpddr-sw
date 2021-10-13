################################################################################
##                        WDDR Interface Library
##
##  This library defines the public interface for WDDR (PHY) Implementation.
##  It defines the PHY Interface, the Memory Map, IRQ Map, PHY Configuration,
##  and the PHY configuration table.
###############################################################################/
add_library(
    wddr_intf
    INTERFACE
)

target_include_directories(
    wddr_intf
    INTERFACE
    ${WAV_WDDR_TOP_LEVEL}/include/dev
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
)

target_link_libraries(
    wddr_intf
    INTERFACE
    kernel
    messenger_driver
)

################################################################################
##                        WDDR Device Library
##
##  This library defines the WDDR device and driver implementations for the PHY.
##  It chooses which drivers and devices are avaiable for the PHY
##  implementation.
###############################################################################/
FILE(
    GLOB_RECURSE
    WDDR_DRIVER_SOURCES
    ${WAV_WDDR_TOP_LEVEL}/drivers/*/*.c
)

FILE(
    GLOB_RECURSE
    WDDR_DEV_SOURCES
    ${WAV_WDDR_TOP_LEVEL}/dev/*/*.c
)

# Exclude WDDR Device (this is built later)
list(
    REMOVE_ITEM
    WDDR_DEV_SOURCES
    ${WAV_WDDR_TOP_LEVEL}/dev/wddr/device.c
)

set(
    WDDR_DRIVER_INCLUDES
    ${WAV_WDDR_TOP_LEVEL}/include/drivers
    ${WAV_WDDR_TOP_LEVEL}/include/dev
    ${WAV_WDDR_TOP_LEVEL}/include/table
    ${WAV_WDDR_TOP_LEVEL}/include
)

add_library(
    wddr_dev
    STATIC
    ${WDDR_DRIVER_SOURCES}
    ${WDDR_DEV_SOURCES}
)

target_include_directories(
    wddr_dev
    PUBLIC
    ${WDDR_DRIVER_INCLUDES}
)

target_compile_options(
    wddr_dev
    PRIVATE
    -Wno-missing-field-initializers
)

target_link_libraries(
    wddr_dev
    PUBLIC
    wddr_intf
)

################################################################################
##                        WDDR Extension Library
##
##  This library extends functionality of PHY Implementation. It can override
##  any PHY Interface functions or the default training algorithms. Futhermore,
##  this library is optional. Thus, users are not required to add any addtional
##  source to the extension.
###############################################################################/
add_custom_command(
    OUTPUT tmp.c
    COMMAND touch tmp.c
)

add_library(
    wddr_ext
    STATIC
    tmp.c
)

target_link_libraries(
    wddr_ext
    PRIVATE
    wddr_dev
)

################################################################################
##                        WDDR Implementation Library
##
##  This library defines the actual PHY interface implementation
###############################################################################/
add_library(
    wddr_impl
    STATIC
    ${WAV_WDDR_TOP_LEVEL}/dev/wddr/device.c
)

target_compile_options(
    wddr_impl
    PRIVATE
    -Wno-missing-field-initializers
)

target_link_libraries(
    wddr_impl
    PRIVATE
    wddr_intf
    wddr_dev
)

##############################################################################
##                              WDDR Library
##
##  This library defines the aggregate PHY implementation. This library
##  is required so that the WDDR extension library can be used to override
##  any weak implementations in wddr_impl library. A limitation of the linker
##  is that it doesn't attemp to resovle weak functions when built. It will
##  simply use the first implementation of the function regardless if it is
##  WEAK. Thus, this library guratnees that the WDDR Extension Library has its
##  implementation (with any potential STRONG functions) defined first so that
##  the linker discovers these first.
###############################################################################/
add_library(
    wddr
    STATIC
    tmp.c
)

target_link_libraries(
    wddr
    PRIVATE
    wddr_dev
    -Wl,--whole-archive
    wddr_ext
    -Wl,--no-whole-archive
    wddr_impl
)
