// Default values for nodeIds
#ifndef NodeIds
#define NodeIds 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
#endif

// Define nodeIds array based on preprocessor directive
const int nodeIds[] = {NodeIds};
const int numSensors = sizeof(nodeIds) / sizeof(nodeIds[0]); // Calculate the number of sensors

// Define GPIO pins corresponding to the nodeIds
const int sensorPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};      // Adjust this array to match your actual sensor pins
const int numPins = sizeof(sensorPins) / sizeof(sensorPins[0]); // Calculate the number of pins

unsigned long previousMillis = 0; // Variable to store the time of the last iteration
unsigned long interval = 100;     // Initial delay interval in milliseconds

// Ensure that the number of nodeIds is less than or equal to the number of pins
#if numSensors > numPins
#error "Number of nodeIds exceeds the number of sensor pins"
#endif

bool prevSensorStates[numSensors] = {false}; // Previous state of sensors

void setup() {
    Serial.begin(115200);

    // Initialize sensor pins as inputs
    for (int i = 0; i < numSensors; i++) {
        pinMode(sensorPins[i], INPUT);
    }
}

void loop() {
    unsigned long currentMillis = millis();                   // Get the current time
    unsigned long deltaTime = currentMillis - previousMillis; // Calculate the time elapsed since the last iteration
                                                              // Check if it's time to perform the next iteration
    if (deltaTime >= interval) {
        // Update the previousMillis for the next iteration
        previousMillis = currentMillis;

        // Poll sensor states
        for (int i = 0; i < numSensors; i++) {
            bool currentSensorState = digitalRead(sensorPins[i]);
            if (currentSensorState != prevSensorStates[i]) {
                // Send MQTT message for the node
                Serial.print("Sending MQTT message for node ");
                Serial.println(nodeIds[i]);
                // Add MQTT message sending logic here
                // Assuming we are using a PubSubClient library, we can publish messages like this:
                // client.publish("sensorState", String(nodeIds[i]).c_str());
                // Remember to handle MQTT connection and message sending in a non-blocking way
                // You may also want to add some delay to avoid sending multiple messages for rapid state changes
                delay(10); // Adjust as needed
                prevSensorStates[i] = currentSensorState;
            }
        }

        // Adjust the interval based on workload or events
        if (someCondition) {
            interval = 50; // Shorter delay if some condition is met
        } else {
            interval = 100; // Default delay
        }
    }

    // Main loop delay
    delay(100); // Adjust as needed for desired loop frequency
}
