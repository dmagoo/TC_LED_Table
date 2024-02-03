add_library(ConfigLib
    make_cluster_config.cpp
    make_mqtt_config.cpp
)

target_include_directories(ConfigLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
