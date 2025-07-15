#include "memory.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>
#include <iomanip>

std::pair<long, float> GetMemoryUsage() { // Return pair: {totalMem, utilization_percentage}
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open /proc/meminfo" << std::endl;
        return {-1, -1.0f}; // Error pair: totalMem=-1, utilization=-1.0
    }
    std::string line, key, valueStr;
    float totalMemFloat = -1.0f, availableMemFloat = -1.0f;
    long totalMemKB = -1;
    while (std::getline(file, line)) {
        std::istringstream linestream(line);
        linestream >> key >> valueStr;
        if (key == "MemTotal:") {
            totalMemFloat = std::stof(valueStr);
            totalMemKB = static_cast<long>(totalMemFloat);
        } else if (key == "MemAvailable:") {
            availableMemFloat = std::stof(valueStr);
        }
    }
    if (totalMemFloat == -1.0f || availableMemFloat == -1.0f) {
        std::cerr << "Error: Could not read MemTotal or MemAvailable from /proc/meminfo" << std::endl;
        return {-1, -1.0f};
    }
    float utilizationPercentage = 0.0f;
    if (totalMemFloat > 0.0f) {
        utilizationPercentage = ((totalMemFloat - availableMemFloat) / totalMemFloat) * 100.0f;
    }
    return {totalMemKB, utilizationPercentage};
}
