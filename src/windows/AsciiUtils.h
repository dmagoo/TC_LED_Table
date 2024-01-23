#ifndef ASCII_UTILS_H
#define ASCII_UTILS_H

#include "core/LedTableTypes.h"
#include "core/Cluster.h"
#include <string>

std::string nodeBufferToAscii(const Cluster& cluster, int nodeId);
std::string convertColorToAnsiCode(RGBW color); // Declaration of the helper function

#endif // ASCII_UTILS_H
