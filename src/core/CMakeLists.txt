add_library(CoreLib
    Cluster.cpp
    ClusterConfig.cpp
    ClusterManager.cpp
    ClusterMessage.cpp
    ClusterMessageManager.cpp
    Node.cpp
    NodeConfig.cpp
    coordinates/Cartesian2dCoordinate.cpp
    coordinates/CubeCoordinate.cpp
    coordinates/RingCoordinate.cpp
    proto/ClusterMessagesProto.pb.c
)

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
