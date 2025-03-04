#include "process.h"
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map> // For std::map

using namespace std;

int GetRunningProcesses() {
    ifstream file("/proc/stat");
    string key;
    int runningProcesses = 0;

    while (file >> key) {
        if (key == "procs_running") {
            file >> runningProcesses;
            break;
        }
    }
    return runningProcesses;
}

// Static map to store previous CPU times for processes
static map<int, pair<long, long>> prevProcessCpuTimes;
// Static variables to store previous system CPU times
static long prevTotalSystemTime = 0;
static long prevIdleSystemTime = 0;


std::vector<ProcessInfo> GetProcessList() {
    std::vector<ProcessInfo> processes;
    DIR *dir;
    struct dirent *ent;

    // Get current system CPU idle and total time
    ifstream statFile("/proc/stat");
    string line;
    getline(statFile, line);
    istringstream iss(line);
    string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long currentIdleSystemTime = idle + iowait;
    long currentTotalSystemTime = user + nice + system + idle + iowait + irq + softirq + steal;

    if (prevTotalSystemTime == 0) { // First call, initialize previous system times
        prevTotalSystemTime = currentTotalSystemTime;
        prevIdleSystemTime = currentIdleSystemTime;
    }

    long totalSystemTimeDiff = currentTotalSystemTime - prevTotalSystemTime;
    long idleSystemTimeDiff = currentIdleSystemTime - prevIdleSystemTime;


    if ((dir = opendir("/proc")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR) {
                if (isdigit(ent->d_name[0])) {
                    int pid = std::stoi(ent->d_name);
                    string comm_file = "/proc/" + string(ent->d_name) + "/comm";
                    ifstream comm_stream(comm_file);
                    string processName;
                    if (comm_stream.is_open()) {
                        getline(comm_stream, processName);
                        if (!processName.empty() && processName.back() == '\n') {
                            processName.pop_back();
                        }

                        string stat_file = "/proc/" + to_string(pid) + "/stat";
                        ifstream stat_stream(stat_file);
                        string stat_line;
                        getline(stat_stream, stat_line);
                        stringstream stat_ss(stat_line);
                        string temp;
                        long utime, stime;
                        for (int i = 1; i <= 14; ++i) {
                            stat_ss >> temp;
                        }
                        stat_ss >> utime >> stime;

                        float cpuUsagePercent = 0.0f;
                        if (prevProcessCpuTimes.count(pid)) { // If we have previous CPU time for this process
                            long prev_utime = prevProcessCpuTimes[pid].first;
                            long prev_stime = prevProcessCpuTimes[pid].second;
                            long processCpuTimeDiff = (utime - prev_utime) + (stime - prev_stime);

                            if (totalSystemTimeDiff > 0) { // Avoid division by zero
                                cpuUsagePercent = (static_cast<float>(processCpuTimeDiff) / static_cast<float>(totalSystemTimeDiff)) * 100.0f;
                            }
                        }
                        processes.push_back({pid, processName, cpuUsagePercent});
                        prevProcessCpuTimes[pid] = {utime, stime}; // Store current times as previous for next iteration


                    } else {
                        cerr << "Error: Could not open " << comm_file << endl;
                    }
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "Error: Could not open /proc directory" << endl;
    }

    // Update previous system CPU times for next iteration
    prevTotalSystemTime = currentTotalSystemTime;
    prevIdleSystemTime = currentIdleSystemTime;


    // Sort processes by CPU usage in descending order
    sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuUsagePercent > b.cpuUsagePercent;
    });

    return processes;
}