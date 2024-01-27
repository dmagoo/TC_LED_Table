#include "api/LedTableApi.h"
#include "config/make_cluster_config.h"
#include "core/Cluster.h"
#include "core/ClusterManager.h"
// #include "core/coordinates/RingCoordinate.h"
#include "core/LedTableTypes.h"
#include "windows/AsciiUtils.h"

#include <signal.h>
#include <vector>
#include <windows.h>
// #include <unistd.h> // For sleep function

#include <iostream>
// #include <string>

#define DELAY_MS 1000
#define NUM_NODES 7

// Global flag to control loop termination
volatile sig_atomic_t keepRunning = 1;

// Signal handler to catch CTRL+C
void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, intHandler); // Register the signal handler

    ClusterManager clusterManager(makeClusterConfigs());
    const Cluster *cluster = clusterManager.getClusterById(0);

    LedTableApi api(clusterManager);

    // Define the color list
    std::vector<RGBW> colors = {0x00000000, 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFFFFFF00, 0xFFFF00FF, 0xFF00FFFF};
    // track the fill from each node so we can move colors to the new node.
    // could do this by getting the buffer, but this is just a cheap demo!
    std::vector<RGBW> nodeColors(NUM_NODES, 0x00000000); // Initialize all nodes to black

    int currentColorIndex = 0;

    while (keepRunning) {
        // Shift colors across nodes
        for (int nodeId = NUM_NODES - 1; nodeId > 0; --nodeId) {
            nodeColors[nodeId] = nodeColors[nodeId - 1];
            api.fillNode(nodeId, nodeColors[nodeId]);
        }

        // Update node 0 with the new color and increment the color index
        nodeColors[0] = colors[currentColorIndex];
        api.fillNode(0, nodeColors[0]);
        currentColorIndex = (currentColorIndex + 1) % colors.size();

        for (int nodeId = 0; nodeId < NUM_NODES; nodeId++) {
            std::string asciiArt = nodeBufferToAscii(*cluster, nodeId);
            std::cout << asciiArt;
        }
        //    asciiArt = nodeBufferToAscii(cluster, 1);
        //    std::cout << asciiArt;

        Sleep(DELAY_MS); // Delay
        // posix: usleep(DELAY_MS * 1000); // Delay
    }

    return 0;
}