#ifndef LED_TABLE_CONFIG_H
#define LED_TABLE_CONFIG_H

#include "make_mqtt_config.h"
#include "make_artnet_config.h"

struct LedTableConfig {
    MQTTConfig mqttConfig;
    ArtnetConfig artnetConfig;
    bool enableMQTTMessaging;
    bool enableMQTTSubscriptions;
    bool enableArtnetMessaging;
};

#endif
