set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project("wddr_main" C ASM)

# Directories
set(APP_DIR         "${CMAKE_CURRENT_LIST_DIR}")

# Source Files
set(SOURCE_FILES
    ${APP_DIR}/main.c
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    -Wno-missing-field-initializers
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${APP_DIR}
    ${WAV_BUILD_TOP_LEVEL}/include
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    -DGIT_SHA=\"${VERSION_GIT_SHA}\"
    -DGIT_DIRTY=${VERSION_DIRTY}
    -DGIT_AHEAD=${VERSION_AHEAD}
    -DFW_VERSION_MAJOR=${VERSION_MAJOR}
    -DFW_VERSION_MINOR=${VERSION_MINOR}
    -DFW_VERSION_PATCH=${VERSION_PATCH}
)

target_link_libraries(
    ${PROJECT_NAME}
    phy_firmware
)

target_link_options(
    ${PROJECT_NAME}
    PUBLIC
    -Wl,--defsym,__stack_size=0x200
    -Wl,--defsym,__heap_size=0x0
    -Wl,--build-id
)

bsp_post_build(${PROJECT_NAME})
