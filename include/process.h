#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>

int GetRunningProcesses();

struct ProcessInfo {
    int pid;
    std::string name;
    float cpuUsagePercent;
};

std::vector<ProcessInfo> GetProcessList();

#endif