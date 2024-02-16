#include "SensorMessage.h"
#include <stdexcept>
#include <vector>

std::vector<uint8_t> SensorMessage::serialize() const {
    uint8_t buffer[1024]; // Adjust the buffer size as necessary
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, SensorEventDataProto_fields, &sensorEventData_)) {
        std::cerr << " Encoding error" << std::endl;
    }

    return std::vector<uint8_t>(buffer, buffer + stream.bytes_written);
}

const SensorEventDataProto SensorMessage::getPayload() const {
    return sensorEventData_;
}

TouchEventMessage::TouchEventMessage() {
    sensorEventData_.event_type = SensorEventDataProto_EventType_TOUCH_EVENT;
}

void TouchEventMessage::setSensorData(uint32_t nodeId, uint32_t currentValue, bool touched) {
    SensorDataProto sensorData;
    sensorData.node_id = nodeId;
    sensorData.current_value = currentValue;
    sensorData.touched = touched;
    sensorEventData_.has_sensor_data = true;
    sensorEventData_.sensor_data = sensorData;
}

PeriodicNotificationMessage::PeriodicNotificationMessage() {
    sensorEventData_.event_type = SensorEventDataProto_EventType_PERIODIC_NOTIFICATION;
}

void PeriodicNotificationMessage::setSensorStatus(uint32_t nodeId, uint32_t thresholdOff, uint32_t thresholdOn) {
    SensorDataProto sensorData;
    sensorData.node_id = nodeId;
    sensorData.threshold_off = thresholdOff;
    sensorData.threshold_on = thresholdOn;
    sensorEventData_.has_sensor_data = true;
    sensorEventData_.sensor_data = sensorData;
}


std::unique_ptr<SensorMessage> deserializeSensorMessage(const std::vector<uint8_t> &buffer) {
    SensorEventDataProto proto_msg = SensorEventDataProto_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());
    if (!pb_decode(&stream, SensorEventDataProto_fields, &proto_msg)) {
        std::cerr << "DECODING ERROR: " << PB_GET_ERROR(&stream) << std::endl;
        throw std::runtime_error("Decoding failed for SensorEventDataProto");
    }

    if (!proto_msg.has_sensor_data) {
        throw std::runtime_error("Sensor Event is missing data");
    }

    if (proto_msg.event_type == SensorEventDataProto_EventType_TOUCH_EVENT) {
        auto touchEventMessage = std::make_unique<TouchEventMessage>();
        touchEventMessage->setSensorData(proto_msg.sensor_data.node_id, proto_msg.sensor_data.current_value, proto_msg.sensor_data.touched);
        touchEventMessage->getPayload();
        return touchEventMessage; // This is fine because std::unique_ptr<TouchEventMessage> can be converted to std::unique_ptr<SensorMessage>
    }

    if (proto_msg.event_type == SensorEventDataProto_EventType_PERIODIC_NOTIFICATION) {
        auto periodicNotificationMessage = std::make_unique<PeriodicNotificationMessage>();
        periodicNotificationMessage->setSensorStatus(proto_msg.sensor_data.node_id, proto_msg.sensor_data.threshold_off, proto_msg.sensor_data.threshold_on);
        return periodicNotificationMessage; // Same as above, conversion is allowed
    }

    return nullptr; // In case none of the conditions are met, return nullptr
}


// Helper function to deserialize from string
std::unique_ptr<SensorMessage> deserializeSensorMessage(const std::string &bufferString) {
    std::vector<uint8_t> bufferVector(bufferString.begin(), bufferString.end());
    return deserializeSensorMessage(bufferVector);
}