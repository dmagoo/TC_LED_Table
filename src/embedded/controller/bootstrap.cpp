#include "bootstrap.h"
#include "Controller.h"
#include <freertos/FreeRTOS.h>
// #include <freertos/task.h>

Controller controller;  // Global Controller instance

// Task function declarations (if needed)
void someControllerTask(void* parameter);

void controllerSetup() {
    controller.setup();

    // Create tasks specific to the controller's functionality
    // For example:
    // xTaskCreatePinnedToCore(someControllerTask, "SomeTask", 10000, NULL, 1, NULL, 0); // Core 0
}

void controllerLoop() {
    // This might be empty or handle low-priority tasks
   // vTaskDelay(1000);
}

void someControllerTask(void* parameter) {
    for (;;) {
        // Controller-specific task logic
    //    vTaskDelay(1); // Delay to prevent task from using 100% CPU
    }
}
