#include "cpu.h"
#include <fstream>
#include <sstream>

using namespace std;

float GetCpuUsage() {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);

    istringstream iss(line);
    string cpu;
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
