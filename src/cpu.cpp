// Implementation for getting cpu usage
#include "cpu.h"
#include <fstream>
#include <sstream>
#include <string>

float GetCpuUsage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    static long prevIdle = 0, prevTotal = 0;
    long idleTime = idle + iowait;
    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
    float cpuUsage = (1.0 - (float)(idleTime - prevIdle) / (totalTime - prevTotal)) * 100.0;
    prevIdle = idleTime;
    prevTotal = totalTime;
    return cpuUsage;
}
