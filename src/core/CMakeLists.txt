add_library(CoreLib
    Cluster.cpp
    ClusterConfig.cpp
    ClusterManager.cpp
    ClusterMessage.cpp
    ClusterMessageManager.cpp
    Node.cpp
    NodeConfig.cpp
    node_geometry.cpp
    SensorListener.cpp
    SensorMessage.cpp
    SensorTransmitter.cpp
    coordinates/Cartesian2dCoordinate.cpp
    coordinates/CubeCoordinate.cpp
    coordinates/RingCoordinate.cpp
    sensors/TouchSensor.cpp
    proto/ClusterMessagesProto.pb.c
    proto/SensorDataProto.pb.c
    )

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
