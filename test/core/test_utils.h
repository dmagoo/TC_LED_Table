#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include "core/LedTableTypes.h"
#include <vector>
#include <cstdint>

void verifyBufferIsFilledWithColor(std::vector<WRGB> buffer, WRGB expectedColor);
void verifyBufferHasColorAt(std::vector<WRGB> buffer, int index, WRGB expectedColor);
void outputBufferAsHex(std::vector<WRGB> buffer);

#endif // TEST_UTILS_H
