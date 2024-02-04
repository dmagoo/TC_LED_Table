cmake_minimum_required(VERSION 3.5)
project(LedTableApiBindings)
set(pybind11_DIR "${CMAKE_SOURCE_DIR}/lib/pybind11/share/cmake/pybind11")

pybind11_add_module(tc_led_table 
    tc_led_table_module.cpp
    coordinate_bindings.cpp
    led_table_api_bindings.cpp  # Keep this if it still contains necessary bindings
)


set_target_properties(tc_led_table PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/python_bindings
)


target_include_directories(tc_led_table PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/api
)


# Link the Python bindings module with the LedTableApi library and MQTT Paho libraries
target_link_libraries(tc_led_table PRIVATE 
    LedTableApi 
    ${PAHO_MQTT_C_LIBRARIES} 
    ${PAHO_MQTT_CPP_LIBRARIES}
    Ws2_32  # Add this to link against the Winsock library
)

# Link the main C++ project library
target_link_libraries(tc_led_table PRIVATE LedTableApi ${Python_LIBRARIES})
