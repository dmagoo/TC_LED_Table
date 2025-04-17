#ifndef WORKER_BOOTSTRAP_H
#define WORKER_BOOTSTRAP_H

#include "Worker.h"

// Task function declarations
void handleMQTTTask(void* parameter);
void handleLEDSensorTask(void* parameter);

void workerSetup();
void workerLoop();

#endif // WORKER_BOOTSTRAP_H
