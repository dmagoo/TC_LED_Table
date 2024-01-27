cmake_minimum_required(VERSION 3.5)
project(TCLEDTable)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Platform-specific definitions
if(WIN32)
    add_definitions(-D_WIN64)
    add_definitions(-DWIN64)
    # Windows-specific include and library paths
    set(PAHO_MQTT_C_INCLUDE_DIRS "C:/DevTools/installed/paho.mqtt.c/include")
    set(PAHO_MQTT_CPP_INCLUDE_DIRS "C:/DevTools/installed/paho.mqtt.cpp/include")
    set(PAHO_MQTT_C_LIBRARIES "C:/DevTools/installed/paho.mqtt.c/lib/paho-mqtt3a-static.lib")
    set(PAHO_MQTT_CPP_LIBRARIES "C:/DevTools/installed/paho.mqtt.cpp/lib/paho-mqttpp3-static.lib")
else()
    # For Linux, use pkg-config or find_package as fallback
    find_package(PkgConfig)
    pkg_check_modules(PAHO_MQTT_C REQUIRED libpaho-mqtt3c)
    pkg_check_modules(PAHO_MQTT_CPP REQUIRED libpaho-mqttpp3)
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib/CLI11/include
    ${PROJECT_SOURCE_DIR}/lib/nanopb
    ${PAHO_MQTT_C_INCLUDE_DIRS}
    ${PAHO_MQTT_CPP_INCLUDE_DIRS}
)

# Source files
file(GLOB_RECURSE CONFIG_SOURCES "${PROJECT_SOURCE_DIR}/src/config/*.cpp")
file(GLOB_RECURSE API_SOURCES "${PROJECT_SOURCE_DIR}/src/api/*.cpp")
file(GLOB_RECURSE WIN_SOURCES "${PROJECT_SOURCE_DIR}/src/windows/*.cpp")
file(GLOB_RECURSE NATIVE_SOURCES "${PROJECT_SOURCE_DIR}/src/core/*.cpp" "${PROJECT_SOURCE_DIR}/src/core/*.c")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Executables
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

# Link libraries
target_link_libraries(demo
    ${PAHO_MQTT_C_LIBRARIES}
    ${PAHO_MQTT_CPP_LIBRARIES}
    $<$<BOOL:${WIN32}>:ws2_32>
)

# For nanopb, specify the source files directly
target_sources(demo PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)
