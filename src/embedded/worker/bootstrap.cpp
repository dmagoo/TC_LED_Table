#include "bootstrap.h"
#include "config/make_cluster_config.h"
#include "config/worker_settings.h"
#include "core/Cluster.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Adafruit_NeoPixel.h>

// increase if stack overflows happen.  Or monitor:
// uxTaskGetStackHighWaterMark
#define TASK_SIZE 4096

static Worker worker(CLUSTER_ID);

void workerSetupOLD() {
    worker.setup();
    xTaskCreatePinnedToCore(handleMQTTTask, "MQTTTask", TASK_SIZE, NULL, 1, NULL, 0);           // Core 0
    xTaskCreatePinnedToCore(handleLEDSensorTask, "LEDSensorTask", TASK_SIZE, NULL, 1, NULL, 1); // Core 1
}

void workerSetup() {
    worker.setup();

    BaseType_t mqttTaskStatus = xTaskCreatePinnedToCore(
        handleMQTTTask, "MQTTTask", TASK_SIZE, NULL, 1, NULL, 0); // Core 0

    BaseType_t ledSensorTaskStatus = xTaskCreatePinnedToCore(
        handleLEDSensorTask, "LEDSensorTask", TASK_SIZE, NULL, 1, NULL, 1); // Core 1

    if (mqttTaskStatus != pdPASS) {
        Serial.println("Failed to create MQTT Task");
        // Handle the error, such as by entering a safe state or retrying task creation
    }

    if (ledSensorTaskStatus != pdPASS) {
        Serial.println("Failed to create LED Sensor Task");
        // Handle the error similarly
    }
}


void workerLoop() {
    // This might be empty or handle low-priority tasks
    worker.loop();
}

void handleMQTTTask(void *parameter) {
    for (;;) {
        worker.handleMQTTMessages();
        // Check stack space remaining
        //UBaseType_t stackWaterMark = uxTaskGetStackHighWaterMark(NULL);
        //Serial.print("MQTTTask Stack remaining: ");
        //Serial.println(stackWaterMark * sizeof(portSTACK_TYPE)); // Multiply by the size of the stack type to get bytes
        vTaskDelay(pdMS_TO_TICKS(1));  // Delay to prevent task from using 100% CPU
    }
}

void handleLEDSensorTask(void *parameter) {
    for (;;) {
        worker.updateLEDs();
        worker.readSensors();
        vTaskDelay(pdMS_TO_TICKS(1)); // Delay to prevent task from using 100% CPU
    }
}
