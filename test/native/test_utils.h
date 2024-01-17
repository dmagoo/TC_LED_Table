#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include "native/LedTableTypes.h"
#include <vector>
#include <cstdint>

void verifyBufferIsFilledWithColor(std::vector<RGBW> buffer, RGBW expectedColor);
void verifyBufferHasColorAt(std::vector<RGBW> buffer, int index, RGBW expectedColor);
void outputBufferAsHex(std::vector<RGBW> buffer);

#endif // TEST_UTILS_H
