// Implementation for getting running processes
#include "process.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <map>
#include <iostream>
#include <utility>
#include <cctype>

int GetRunningProcesses() {
    std::ifstream file("/proc/stat");
    std::string key;
    int runningProcesses = 0;
    while (file >> key) {
        if (key == "procs_running") {
            file >> runningProcesses;
            break;
        }
    }
    return runningProcesses;
}

std::vector<ProcessInfo> GetProcessList() {
    static std::map<int, std::pair<long, long>> prevProcessCpuTimes;
    static long prevTotalSystemTime = 0;
    static long prevIdleSystemTime = 0;

    std::vector<ProcessInfo> processes;
    DIR *dir;
    struct dirent *ent;

    // Get current system CPU idle and total time
    std::ifstream statFile("/proc/stat");
    std::string line;
    std::getline(statFile, line);
    std::istringstream iss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long currentIdleSystemTime = idle + iowait;
    long currentTotalSystemTime = user + nice + system + idle + iowait + irq + softirq + steal;

    if (prevTotalSystemTime == 0) {
        prevTotalSystemTime = currentTotalSystemTime;
        prevIdleSystemTime = currentIdleSystemTime;
    }

    long totalSystemTimeDiff = currentTotalSystemTime - prevTotalSystemTime;

    if ((dir = opendir("/proc")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && std::isdigit(ent->d_name[0])) {
                int pid = std::stoi(ent->d_name);

                // Get process name
                std::string comm_file = "/proc/" + std::string(ent->d_name) + "/comm";
                std::ifstream comm_stream(comm_file);
                std::string processName;
                if (comm_stream.is_open()) {
                    std::getline(comm_stream, processName);
                    if (!processName.empty() && processName.back() == '\n') {
                        processName.pop_back();
                    }
                } else {
                    continue; // Skip if cannot open comm file
                }

                // Get process CPU times
                std::string stat_file = "/proc/" + std::to_string(pid) + "/stat";
                std::ifstream stat_stream(stat_file);
                std::string stat_line;
                std::getline(stat_stream, stat_line);
                std::stringstream stat_ss(stat_line);
                std::string temp;
                long utime = 0, stime = 0;
                for (int i = 1; i <= 13; ++i) {
                    stat_ss >> temp;
                }
                stat_ss >> utime >> stime;

                float cpuUsagePercent = 0.0f;
                if (prevProcessCpuTimes.count(pid)) {
                    long prev_utime = prevProcessCpuTimes[pid].first;
                    long prev_stime = prevProcessCpuTimes[pid].second;
                    long processCpuTimeDiff = (utime - prev_utime) + (stime - prev_stime);
                    if (totalSystemTimeDiff > 0) {
                        cpuUsagePercent = (static_cast<float>(processCpuTimeDiff) / totalSystemTimeDiff) * 100.0f;
                    }
                }

                // Get RAM Usage (VmRSS)
                std::string status_file = "/proc/" + std::to_string(pid) + "/status";
                std::ifstream status_stream(status_file);
                std::string status_line;
                long ramUsage = 0;
                while (std::getline(status_stream, status_line)) {
                    if (status_line.substr(0, 6) == "VmRSS:") {
                        std::string value_str;
                        std::stringstream line_stream(status_line);
                        std::string key;
                        line_stream >> key >> value_str;
                        try {
                            ramUsage = std::stol(value_str);
                        } catch (const std::invalid_argument&) {
                            ramUsage = 0;
                        }
                        break;
                    }
                }

                processes.push_back({pid, processName, cpuUsagePercent, ramUsage});
                prevProcessCpuTimes[pid] = {utime, stime};
            }
        }
        closedir(dir);
    }

    prevTotalSystemTime = currentTotalSystemTime;
    prevIdleSystemTime = currentIdleSystemTime;

    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuUsagePercent > b.cpuUsagePercent;
    });

    return processes;
}
