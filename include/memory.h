#ifndef MEMORY_H
#define MEMORY_H

#include <utility>

// returns pair {total memory kb,utilization_percentage}
std::pair<long, float> GetMemoryUsage();
float GetMemoryUtilization();

#endif