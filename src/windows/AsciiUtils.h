#ifndef ASCII_UTILS_H
#define ASCII_UTILS_H

#include "native/Cluster.h"
#include <string>

std::string nodeBufferToAscii(const Cluster& cluster, int nodeId);
std::string convertColorToAnsiCode(int32_t color); // Declaration of the helper function

#endif // ASCII_UTILS_H
