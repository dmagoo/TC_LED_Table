cmake_minimum_required(VERSION 3.5)

# cmake --build . --target <your_python_bindings_target_name>

project(TCLEDTable)
enable_testing()
# run with: ctest -C Debug

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/lib/CLI11/include
    ${PROJECT_SOURCE_DIR}/lib/nanopb
    ${PROJECT_SOURCE_DIR}/test/core/test_basic  # Test headers
    ${PROJECT_SOURCE_DIR}/lib/Unity/src  # Unity framework
)

if(WIN32)
    add_definitions(-D_WIN64)
    add_definitions(-DWIN64)

    # Specify Windows-specific include and library paths if not using standard installation paths
    include_directories("C:/DevTools/installed/paho.mqtt.c/include" "C:/DevTools/installed/paho.mqtt.cpp/include" "C:/DevTools/libartnet" "C:/DevTools/libartnet/artnet")
    set(PAHO_MQTT_C_LIBRARIES "C:/DevTools/installed/paho.mqtt.c/lib/paho-mqtt3a-static.lib")
    set(PAHO_MQTT_CPP_LIBRARIES "C:/DevTools/installed/paho.mqtt.cpp/lib/paho-mqttpp3-static.lib")
    set(ARTNET_LIBRARIES "C:/DevTools/libartnet/msvc/libartnet/x64/Release/libartnet.lib")
endif()

add_subdirectory(src/api)
add_subdirectory(src/config)
add_subdirectory(src/core)
add_subdirectory(src/windows)

add_library(nanopb STATIC
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)
target_include_directories(nanopb PUBLIC ${PROJECT_SOURCE_DIR}/lib/nanopb)

# Source files
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/core/test_basic/*.cpp")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_test(NAME unity_test COMMAND test_app)

add_executable(demo
    ${PROJECT_SOURCE_DIR}/src/native/main.cpp
    ${CONFIG_SOURCES}
)
target_link_libraries(demo
    ConfigLib
    CoreLib
    nanopb
    $<$<PLATFORM_ID:Windows>:${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES} ws2_32>
    $<$<NOT:$<PLATFORM_ID:Windows>>:PahoMqttCpp::paho-mqttpp3-static>
)

add_executable(test_app
    ${TEST_SOURCES}
    ${PROJECT_SOURCE_DIR}/test/core/test_utils.cpp
    ${PROJECT_SOURCE_DIR}/lib/Unity/src/unity.c
)

# Linking ApiLib and conditionally linking platform-specific libraries
target_link_libraries(test_app PRIVATE 
    ApiLib
    ConfigLib
    CoreLib
    nanopb
    $<$<PLATFORM_ID:Windows>:WinLib ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES}  ${ARTNET_LIBRARIES} ws2_32>
    $<$<NOT:$<PLATFORM_ID:Windows>>:PahoMqttCpp::paho-mqttpp3-static>
)

add_executable(api_test
    ${PROJECT_SOURCE_DIR}/src/native/api_test.cpp
)
target_link_libraries(api_test PRIVATE
    ApiLib
    CoreLib
    ConfigLib
    nanopb
    $<$<PLATFORM_ID:Windows>:WinLib ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES}  ${ARTNET_LIBRARIES} ws2_32>
    $<$<NOT:$<PLATFORM_ID:Windows>>:PahoMqttCpp::paho-mqttpp3-static>
)

add_executable(sensor_test
    ${PROJECT_SOURCE_DIR}/src/native/sensor_test.cpp
)
target_link_libraries(sensor_test PRIVATE
    CoreLib
    ConfigLib
    nanopb
    $<$<PLATFORM_ID:Windows>:WinLib ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES}  ${ARTNET_LIBRARIES} ws2_32>
    $<$<NOT:$<PLATFORM_ID:Windows>>:PahoMqttCpp::paho-mqttpp3-static>
)

add_library(LedTableApi "${PROJECT_SOURCE_DIR}/src/api/LedTableApi.cpp")
target_link_libraries(LedTableApi PRIVATE
    ApiLib
    CoreLib
    ConfigLib
    nanopb
    $<$<PLATFORM_ID:Windows>:WinLib  ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES} ${ARTNET_LIBRARIES} ws2_32>
    $<$<NOT:$<PLATFORM_ID:Windows>>:PahoMqttCpp::paho-mqttpp3-static>
)

# Platform-specific definitions and libraries
if(WIN32)
else()
    # Find Paho MQTT C and C++ libraries installed in standard locations
    #temporary fix for weird setup.. should probably make this some kind of env var or flag
    find_package(eclipse-paho-mqtt-c REQUIRED)
    find_package(PahoMqttCpp REQUIRED)
endif()

add_subdirectory(lib/pybind11)
add_subdirectory(src/bindings)
