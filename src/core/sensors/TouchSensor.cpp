#include "TouchSensor.h"
#include <cassert>

TouchSensor::TouchSensor(int nodeId, int thresholdOff, int thresholdOn)
    : nodeId_(nodeId), threshold_off_(thresholdOff), threshold_on_(thresholdOn),
      touched_(false), currentValue_(0) {}

bool TouchSensor::updateTouchedState(int newValue) {
    if (newValue >= threshold_on_) {
        return true;
    }
    if (newValue <= threshold_off_) {
        return false;
    }
    return touched_;
}

bool TouchSensor::setValue(int value) {
    assert(value >= MIN_TOUCH_VALUE && value <= MAX_TOUCH_VALUE);
    currentValue_ = value;
    touched_ = updateTouchedState(currentValue_);
    return touched_;
}

bool TouchSensor::updateThresholds(int thresholdOff, int thresholdOn) {
    threshold_off_ = thresholdOff;
    threshold_on_ = thresholdOn;
    // refresh the state based on new thresholds

    touched_ = updateTouchedState(currentValue_);
    return touched_;
}

bool TouchSensor::getTouchedState() const {
    return touched_;
}

int TouchSensor::getValue() const {
    return currentValue_;
}
