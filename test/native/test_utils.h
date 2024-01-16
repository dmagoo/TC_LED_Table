#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <vector>
#include <cstdint>

void verifyBufferIsFilledWithColor(std::vector<int32_t> buffer, int32_t expectedColor);
void verifyBufferHasColorAt(std::vector<int32_t> buffer, int index, int32_t expectedColor);

#endif // TEST_UTILS_H
