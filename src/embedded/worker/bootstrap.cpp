#include "bootstrap.h"
#include "config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

Worker worker;  // Global Worker instance

void workerSetup() {
    worker.setup();

    // Create tasks for MQTT handling and LED/sensor operations
    xTaskCreatePinnedToCore(handleMQTTTask, "MQTTTask", 10000, NULL, 1, NULL, 0); // Core 0
    xTaskCreatePinnedToCore(handleLEDSensorTask, "LEDSensorTask", 10000, NULL, 1, NULL, 1); // Core 1
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
