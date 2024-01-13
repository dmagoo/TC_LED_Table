#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
    void setup();
    void parseConfig();
    void handleMQTTMessages();
    void updateLEDs();
    void readSensors();
};

#endif // CONTROLLER_H