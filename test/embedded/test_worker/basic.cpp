#include <Arduino.h>
#include <unity.h>

#include "embedded/worker/Worker.h"

// Path to the JSON file in your test directory
const char* jsonFilePath = "test_config.json";

void setUp(void) {
    // This will run before each test
}

void tearDown(void) {
    // This will run after each test
}

void test_worker_setup() {
    //Worker myWorker;
    //myWorker.setup(jsonFilePath);

    // Replace this with actual tests
    //TEST_ASSERT_TRUE(myWorker.isClusterSetupCorrectly());
    // TEST_ASSERT_TRUE();
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_worker_setup);
    UNITY_END();
}

void loop() {
    // Do nothing here
}