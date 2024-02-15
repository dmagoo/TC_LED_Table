#include "core/sensors/TouchSensor.h"
#include <iostream>
#include <unity.h>

void test_touch_sensor_thresholds() {
    TouchSensor sensor(1, 10, 90);
    bool touched = sensor.setValue(80);

    // new value didn't pass uppher threshold
    TEST_ASSERT_FALSE(touched);

    // we passed the threshold
    touched = sensor.setValue(99);
    TEST_ASSERT_TRUE(touched);

    // we went low, but not to the lower theshold
    touched = sensor.setValue(11);
    TEST_ASSERT_TRUE(touched);

    // we dipped low enough
    touched = sensor.setValue(10);
    TEST_ASSERT_FALSE(touched);

    // not high enough...

    touched = sensor.setValue(80);
    TEST_ASSERT_FALSE(touched);

    // but we lowered the threshold
    touched = sensor.updateThresholds(40, 60);
    TEST_ASSERT_TRUE(touched);

    TEST_ASSERT_EQUAL_UINT32(1, 100);
}

int run_basic_sensor_tests(int argc, char **argv) {
    RUN_TEST(test_touch_sensor_thresholds);
    return 0;
}