#include "bootstrap.h"
#include "config/make_cluster_config.h"
#include "native/Cluster.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Adafruit_NeoPixel.h>

#define DEFAULT_WORKER_CLUSTER_ID 0

#if defined(WORKER_CLUSTER_ID)
const int clusterId = WORKER_CLUSTER_ID;
#else
const int clusterId = DEFAULT_WORKER_CLUSTER_ID; // Define a default value
#endif

// increase if stack overflows happen.  Or monitor:
// uxTaskGetStackHighWaterMark
#define TASK_SIZE 2048

static Worker worker(clusterId);

void workerSetup() {
    worker.setup();
    xTaskCreatePinnedToCore(handleMQTTTask, "MQTTTask", TASK_SIZE, NULL, 1, NULL, 0);           // Core 0
    xTaskCreatePinnedToCore(handleLEDSensorTask, "LEDSensorTask", TASK_SIZE, NULL, 1, NULL, 1); // Core 1
}

void workerLoop() {
    // This might be empty or handle low-priority tasks
}

void handleMQTTTask(void *parameter) {
    for (;;) {
        worker.handleMQTTMessages();
        vTaskDelay(1); // Delay to prevent task from using 100% CPU
    }
}

void handleLEDSensorTask(void *parameter) {
    for (;;) {
        worker.updateLEDs();
        worker.readSensors();
        vTaskDelay(1); // Delay to prevent task from using 100% CPU
    }
}
