#include "config.h"

#ifdef UNIT_TEST
    #include <fstream>
    #include <sstream>
#else
    #include <ArduinoJson.h>
    #include <FS.h>
    #include <LittleFS.h>
#endif

#ifdef UNIT_TEST
std::string readFileContentsWindows(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
#else
std::string readFileContentsESP(const char* filename) {
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }

    size_t size = file.size();
    if (size > 1024) {
        Serial.println("File size is too large");
        return "";
    }

    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);
    file.close();
    return std::string(buf.get(), size);
}
#endif

DynamicJsonDocument readJsonFile(const char* filename) {
    std::string content;

    #ifdef UNIT_TEST
    content = readFileContentsWindows(filename);
    #else
    content = readFileContentsESP(filename);
    #endif

    DynamicJsonDocument doc(1024);
    auto error = deserializeJson(doc, content);
    if (error) {
        Serial.println("Failed to parse JSON");
        return DynamicJsonDocument(0);
    }

    return doc;
}

int testFunction() { return 42; }