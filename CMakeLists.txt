cmake_minimum_required(VERSION 3.5)
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


# Source files
file(GLOB_RECURSE CONFIG_SOURCES "${PROJECT_SOURCE_DIR}/src/config/*.cpp")
file(GLOB_RECURSE API_SOURCES "${PROJECT_SOURCE_DIR}/src/api/*.cpp")
file(GLOB_RECURSE WIN_SOURCES "${PROJECT_SOURCE_DIR}/src/windows/*.cpp")
file(GLOB_RECURSE CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/core/*.cpp" "${PROJECT_SOURCE_DIR}/src/core/*.c")
file(GLOB_RECURSE CONFIG_SOURCES "${PROJECT_SOURCE_DIR}/src/config/*.cpp" "${PROJECT_SOURCE_DIR}/src/config/*.c")
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/core/test_basic/*.cpp")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_test(NAME unity_test COMMAND test_app)

add_executable(demo
    ${PROJECT_SOURCE_DIR}/src/native/main.cpp
    ${CONFIG_SOURCES}
    ${CORE_SOURCES}
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)

# Executables
add_executable(test_app
    #${PROJECT_SOURCE_DIR}/test/core/test_basic/main.cpp
    ${TEST_SOURCES}
    ${PROJECT_SOURCE_DIR}/test/core/test_utils.cpp
    ${CONFIG_SOURCES}
    ${CORE_SOURCES}
    ${API_SOURCES}
    ${WIN_SOURCES}
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
    ${PROJECT_SOURCE_DIR}/lib/Unity/src/unity.c
)

add_executable(api_test
    ${PROJECT_SOURCE_DIR}/src/native/api_test.cpp
    #${CONFIG_SOURCES} # don't use this unless mqtt is needed.. lol
    "${PROJECT_SOURCE_DIR}/src/config/make_cluster_config.cpp"
    "${PROJECT_SOURCE_DIR}/src/config/make_mqtt_config.cpp"
    ${CORE_SOURCES}
    ${API_SOURCES}
    ${WIN_SOURCES}
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)

# Platform-specific definitions and libraries
if(WIN32)
    add_definitions(-D_WIN64)
    add_definitions(-DWIN64)
    
    # Specify Windows-specific include and library paths if not using standard installation paths
    include_directories("C:/DevTools/installed/paho.mqtt.c/include" "C:/DevTools/installed/paho.mqtt.cpp/include")
    set(PAHO_MQTT_C_LIBRARIES "C:/DevTools/installed/paho.mqtt.c/lib/paho-mqtt3a-static.lib")
    set(PAHO_MQTT_CPP_LIBRARIES "C:/DevTools/installed/paho.mqtt.cpp/lib/paho-mqttpp3-static.lib")
    
    target_link_libraries(demo ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES} ws2_32)
    target_link_libraries(test_app ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES} ws2_32)
    target_link_libraries(api_test ${PAHO_MQTT_C_LIBRARIES} ${PAHO_MQTT_CPP_LIBRARIES} ws2_32)
    else()
    # Find Paho MQTT C and C++ libraries installed in standard locations
    #temporary fix for weird setup.. should probably make this some kind of env var or flag
    #find_package(PahoMqttC REQUIRED)
    find_package(eclipse-paho-mqtt-c REQUIRED)
    find_package(PahoMqttCpp REQUIRED)
    
    target_link_libraries(demo PahoMqttCpp::paho-mqttpp3-static)
    target_link_libraries(test_app PahoMqttCpp::paho-mqttpp3-static)
    target_link_libraries(api_test PahoMqttCpp::paho-mqttpp3-static)
    endif()

# For nanopb, specify the source files directly
target_sources(demo PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_common.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_decode.c
    ${PROJECT_SOURCE_DIR}/lib/nanopb/pb_encode.c
)
