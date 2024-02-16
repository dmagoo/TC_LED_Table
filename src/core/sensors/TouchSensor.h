#pragma once

#define MIN_TOUCH_VALUE 0
#define MAX_TOUCH_VALUE 100

class TouchSensor {
private:
    int nodeId_;
    int currentValue_;
    bool touched_;

    // Data sent periodically
    int threshold_off_;
    int threshold_on_;

    // set the new sensor value and return updated touch state
    // state only changes when new value is outside of threshold
    // range
    bool updateTouchedState(int newValue);

public:
    TouchSensor(int nodeId, int thresholdOff = 30, int thresholdOn = 60);
    bool updateThresholds(int thresholdOff, int thresholdOn);
    bool setValue(int value);
    bool getTouchedState() const;
    int getValue() const;
};
