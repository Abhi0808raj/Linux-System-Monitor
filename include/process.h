#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>

int GetRunningProcesses();

// Struct to hold process information
struct ProcessInfo {
    int pid;
    std::string name;
};

std::vector<ProcessInfo> GetProcessList();

#endif