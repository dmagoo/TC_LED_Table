#include "bootstrap.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "config/make_cluster_config.h"
#include "native/Cluster.h"

#include <Adafruit_NeoPixel.h>

#define DEFAULT_WORKER_CLUSTER_ID 0

#if defined(WORKER_CLUSTER_ID)
    const int clusterId = WORKER_CLUSTER_ID;
#else
    const int clusterId = DEFAULT_WORKER_CLUSTER_ID; // Define a default value
#endif

static Worker worker(clusterId);

void workerSetup() {
    xTaskCreatePinnedToCore(handleMQTTTask, "MQTTTask", 10000, NULL, 1, NULL, 0); // Core 0
    xTaskCreatePinnedToCore(handleLEDSensorTask, "LEDSensorTask", 10000, NULL, 1, NULL, 1); // Core 1
    worker.setup();
}

void workerLoop() {
    // This might be empty or handle low-priority tasks
}

void handleMQTTTask(void* parameter) {
    for (;;) {
        worker.handleMQTTMessages();
        vTaskDelay(1); // Delay to prevent task from using 100% CPU
    }
}

void handleLEDSensorTask(void* parameter) {
    for (;;) {
        worker.updateLEDs();
        worker.readSensors();
        vTaskDelay(1); // Delay to prevent task from using 100% CPU
    }
}
