#pragma once

#include "core/proto/SensorDataProto.pb.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Abstract base class for sensor messages
class SensorMessage {
public:
    virtual ~SensorMessage() {}
    std::vector<uint8_t> serialize() const;
    const SensorEventDataProto getPayload() const;
    uint32_t SensorMessage::debug() const;

protected:
    SensorEventDataProto sensorEventData_;
};

// Subclass for touch event messages
class TouchEventMessage : public SensorMessage {
public:
    // Constructor
    TouchEventMessage();

    // Method to add sensor data point to the message
    void setSensorData(uint32_t nodeId, uint32_t currentValue, bool touched);
};

// Subclass for periodic notification messages
class PeriodicNotificationMessage : public SensorMessage {
public:
    // Constructor
    PeriodicNotificationMessage();

    // Method to add sensor data point to the message
    void setSensorStatus(uint32_t nodeId, uint32_t thresholdOff, uint32_t thresholdOn);
};

std::unique_ptr<SensorMessage> deserializeSensorMessage(const std::vector<uint8_t> &data);
