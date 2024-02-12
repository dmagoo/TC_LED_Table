add_library(ConfigLib
    led_table_config.cpp
    make_cluster_config.cpp
    make_mqtt_config.cpp
    make_artnet_config.cpp
)

target_include_directories(ConfigLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
