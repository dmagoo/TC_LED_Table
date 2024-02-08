#ifndef LED_TABLE_CONFIG_H
#define LED_TABLE_CONFIG_H

#include "make_mqtt_config.h"

struct LedTableConfig {
    MQTTConfig mqttConfig;
    // Other configuration settings
    bool enableMessaging;
    // Add more fields as needed
};

#endif
