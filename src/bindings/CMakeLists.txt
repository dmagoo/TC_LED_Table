cmake_minimum_required(VERSION 3.5)
project(LedTableApiBindings)
set(pybind11_DIR "${CMAKE_SOURCE_DIR}/lib/pybind11/share/cmake/pybind11")

pybind11_add_module(ledtableapi_py led_table_api_bindings.cpp)

target_include_directories(ledtableapi_py PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/api
)


# Link the Python bindings module with the LedTableApi library and MQTT Paho libraries
target_link_libraries(ledtableapi_py PRIVATE 
    LedTableApi 
    ${PAHO_MQTT_C_LIBRARIES} 
    ${PAHO_MQTT_CPP_LIBRARIES}
    Ws2_32  # Add this to link against the Winsock library
)

# Link the main C++ project library
target_link_libraries(ledtableapi_py PRIVATE LedTableApi ${Python_LIBRARIES})
