#include "test_utils.h"
#include <iostream>
#include <unity.h>

void verifyBufferIsFilledWithColor(std::vector<int32_t> buffer, int32_t expectedColor) {
    for (int i = 0; i < buffer.size(); ++i) {
        TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[i]);
    }
}

void verifyBufferHasColorAt(std::vector<int32_t> buffer, int index, int32_t expectedColor) {
    TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[index]);
}
