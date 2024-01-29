#ifndef MAKE_MQTT_CONFIG_H
#define MAKE_MQTT_CONFIG_H

#include "mqtt/async_client.h"
#include <memory>

std::unique_ptr<mqtt::async_client> makeMQTTClientConfig();

#endif // MAKE_MQTT_CONFIG_H
