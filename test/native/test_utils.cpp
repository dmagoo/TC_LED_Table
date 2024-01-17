#include "test_utils.h"
#include <iostream>
#include <iomanip>
#include <unity.h>

void verifyBufferIsFilledWithColor(std::vector<RGBW> buffer, RGBW expectedColor) {
    for (int i = 0; i < buffer.size(); ++i) {
        TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[i]);
    }
}

void verifyBufferHasColorAt(std::vector<RGBW> buffer, int index, RGBW expectedColor) {
    TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[index]);
}

void outputBufferAsHex(std::vector<RGBW> buffer) {
   std::cout << '{';
   for (int i = 0; i < buffer.size(); ++i) {
        std::cout << " 0x" << std::hex << std::setw(8) << std::setfill('0') << buffer[i];
        if(i<buffer.size()) {
    std::cout << ',';        
        }
   }
    std::cout << '}' << std::endl;
}