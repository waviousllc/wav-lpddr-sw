project("WAV-LPDDR-SW" C ASM)
cmake_minimum_required(VERSION 3.13)

# Set the output folder where your program will be created
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Make sure everything is built for correct architecture
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

set(RTOS_HEAP_ALLOC_SCHEME "1")

include(${CMAKE_CURRENT_LIST_DIR}/rtos/CMakeLists.txt)

add_subdirectory(drivers)
add_subdirectory(dev)
add_subdirectory(fsm)
add_subdirectory(app)
