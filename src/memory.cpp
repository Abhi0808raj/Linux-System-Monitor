#include "memory.h"
#include <fstream>
#include <string>
#include <sstream> // For istringstream
#include <iostream> // For cerr
#include <iomanip>  // For std::fixed and std::setprecision

using namespace std;

pair<long, float> GetMemoryUsage() { // Return pair: {totalMem, utilization_percentage}
    ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        cerr << "Error: Could not open /proc/meminfo" << endl;
        return {-1, -1.0f}; // Error pair: totalMem=-1, utilization=-1.0
    }

    string line, key, valueStr; // valueStr to hold string value before conversion
    float totalMemFloat = -1.0f, availableMemFloat = -1.0f; // Use floats for calculation
    long totalMemKB = -1;

    while (getline(file, line)) { // Read line by line
        istringstream linestream(line);
        linestream >> key >> valueStr; // Read key and value as string
        if (key == "MemTotal:") {
            totalMemFloat = stof(valueStr); // Convert to float using stof
            totalMemKB = static_cast<long>(totalMemFloat); // Keep totalMemKB as long for display
        } else if (key == "MemAvailable:") {
            availableMemFloat = stof(valueStr); // Convert to float using stof
        }
    }

    if (totalMemFloat == -1.0f || availableMemFloat == -1.0f) {
        cerr << "Error: Could not read MemTotal or MemAvailable from /proc/meminfo" << endl;
        return {-1, -1.0f}; // Error pair
    }

    float utilizationPercentage = 0.0f;
    if (totalMemFloat > 0.0f) { // Prevent division by zero
        utilizationPercentage = ((totalMemFloat - availableMemFloat) / totalMemFloat) * 100.0f;
    }

    cerr << "Debug memory.cpp: MemTotal=" << totalMemKB << ", MemAvailable=" << availableMemFloat << " kB, Utilization=" << fixed << setprecision(2) << utilizationPercentage << "%" << endl; // Debug print

    return {totalMemKB, utilizationPercentage}; // Return totalMemKB and utilization percentage
}