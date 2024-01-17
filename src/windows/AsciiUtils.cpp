#include "AsciiUtils.h"
#include <sstream>
#include <iomanip>

// Function to convert the color to an ANSI escape code
std::string convertColorToAnsiCode(RGBW color) {
    // Your color conversion logic goes here
    // For example, using basic primary colors:
    if (color & 0x00FF0000) { // Red component is present
        return "\033[31m"; // Red
    } else if (color & 0x0000FF00) { // Green component is present
        return "\033[32m"; // Green
    } else if (color & 0x000000FF) { // Blue component is present
        return "\033[34m"; // Blue
    }
    return "\033[0m"; // Reset to default (if the color is off)
}

// Function to convert the color to a 24-bit ANSI escape code (True Color)
std::string convertColorToAnsiCodeWIP(RGBW color) {
    int red = (color >> 16) & 0xFF;  // Extract the red component
    int green = (color >> 8) & 0xFF;  // Extract the green component
    int blue = color & 0xFF;          // Extract the blue component

    std::ostringstream ansiCode;

// ansiCode << "\x1b[" << 0 << ";2;" << red << ";" << green << ";" << blue << "mz";

    ansiCode << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
    return ansiCode.str();
}

std::string nodeBufferToAscii(const Cluster& cluster, int nodeId) {
    auto nodeBuffer = cluster.getNodePixelBuffer(nodeId);
    if (nodeBuffer.size() != 8) {
        return "Error: Node does not have 8 LEDs or node not found\n";
    }

    std::ostringstream asciiArt;
    // ... (rest of the function implementation, same as before) ...

    // Top row (North, North-East, East)
    asciiArt << convertColorToAnsiCode(nodeBuffer[7]) <<  std::setw(3) << (nodeBuffer[7] ? 'x' : 'o') << " ";
    asciiArt << convertColorToAnsiCode(nodeBuffer[0]) <<  std::setw(3) << (nodeBuffer[0] ? 'x' : 'o') << " ";
    asciiArt << convertColorToAnsiCode(nodeBuffer[1]) <<  std::setw(3) << (nodeBuffer[1] ? 'x' : 'o') << "\n";

    // Middle row (West, Node ID, East)
    asciiArt << convertColorToAnsiCode(nodeBuffer[6]) <<  std::setw(3) << (nodeBuffer[6] ? 'x' : 'o') << " ";
    asciiArt << std::setw(3) << nodeId << " ";
    asciiArt << convertColorToAnsiCode(nodeBuffer[2]) <<  std::setw(3) << (nodeBuffer[2] ? 'x' : 'o') << "\n";

    // Bottom row (South-West, South, South-East)
    asciiArt << convertColorToAnsiCode(nodeBuffer[5]) <<  std::setw(3) << (nodeBuffer[5] ? 'x' : 'o') << " ";
    asciiArt << convertColorToAnsiCode(nodeBuffer[4]) <<  std::setw(3) << (nodeBuffer[4] ? 'x' : 'o') << " ";
    asciiArt << convertColorToAnsiCode(nodeBuffer[3]) <<  std::setw(3) << (nodeBuffer[3] ? 'x' : 'o') << "\n";

    // Reset to default color at the end
    asciiArt << "\033[0m";

    return asciiArt.str();
}

