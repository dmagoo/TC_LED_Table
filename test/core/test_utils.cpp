#include "test_utils.h"
#include <iomanip>
#include <iostream>
#include <unity.h>

void verifyBufferIsFilledWithColor(std::vector<WRGB> buffer, WRGB expectedColor) {
    for (int i = 0; i < buffer.size(); ++i) {
        TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[i]);
    }
}

void verifyBufferHasColorAt(std::vector<WRGB> buffer, int index, WRGB expectedColor) {
    TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[index]);
}

void outputBufferAsHex(std::vector<WRGB> buffer) {
    std::cout << "size: " << buffer.size() << ' ';
    std::cout << '{';
    for (int i = 0; i < buffer.size(); ++i) {
        std::cout << " 0x" << std::hex << std::setw(8) << std::setfill('0') << buffer[i];
        if (i < buffer.size()) {
            std::cout << ',';
        }
    }
    std::cout << '}' << std::endl;
}