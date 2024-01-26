cmake_minimum_required(VERSION 3.5)
project(TCLEDTable)

# Set the C++ standard you want to use (you mentioned gnu++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Define flags for Windows
add_definitions(-D_WIN64)
add_definitions(-DWIN64)

# Include directories for your project
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib/CLI11/include
    ${PROJECT_SOURCE_DIR}/lib/nanopb
    C:/DevTools/installed/paho.mqtt.c/include
    C:/DevTools/installed/paho.mqtt.cpp/include
)

file(GLOB_RECURSE CONFIG_SOURCES "${PROJECT_SOURCE_DIR}/src/config/*.cpp")
file(GLOB_RECURSE API_SOURCES "${PROJECT_SOURCE_DIR}/src/api/*.cpp")
file(GLOB_RECURSE WIN_SOURCES "${PROJECT_SOURCE_DIR}/src/windows/*.cpp")
file(GLOB_RECURSE NATIVE_SOURCES "${PROJECT_SOURCE_DIR}/src/core/*.cpp"  "${PROJECT_SOURCE_DIR}/src/core/*.c")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(demo
    ${PROJECT_SOURCE_DIR}/src/native/main.cpp
    ${CONFIG_SOURCES}
    ${NATIVE_SOURCES}
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)

add_executable(api_test
    ${PROJECT_SOURCE_DIR}/src/native/api_test.cpp
    ${CONFIG_SOURCES}
    ${NATIVE_SOURCES}
    ${API_SOURCES}
    ${WIN_SOURCES}
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)


target_link_libraries(demo
    C:/DevTools/installed/paho.mqtt.c/lib/paho-mqtt3a-static.lib
    C:/DevTools/installed/paho.mqtt.cpp/lib/paho-mqttpp3-static.lib
    ws2_32
)


# For nanopb, specify the source files directly
target_sources(demo PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)

# If you have any other specific compiler flags, you can add them here
# target_compile_options(demo PRIVATE -lstdc++)

