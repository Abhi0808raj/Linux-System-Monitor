#include "process.h"
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

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

static map<int, pair<long, long>> prevProcessCpuTimes;
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

    if (prevTotalSystemTime == 0) {
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
                        if (prevProcessCpuTimes.count(pid)) {
                            long prev_utime = prevProcessCpuTimes[pid].first;
                            long prev_stime = prevProcessCpuTimes[pid].second;
                            long processCpuTimeDiff = (utime - prev_utime) + (stime - prev_stime);

                            if (totalSystemTimeDiff > 0) {
                                cpuUsagePercent = (static_cast<float>(processCpuTimeDiff) / static_cast<float>(totalSystemTimeDiff)) * 100.0f;
                            }
                        }

                        // Get RAM Usage (VmRSS)
                        string status_file = "/proc/" + to_string(pid) + "/status";
                        ifstream status_stream(status_file);
                        string status_line;
                        long ramUsage = 0; // Default to 0 if not found
                        while (getline(status_stream, status_line)) {
                            if (status_line.substr(0, 6) == "VmRSS:") {
                                string value_str;
                                stringstream line_stream(status_line);
                                string key;
                                line_stream >> key >> value_str;
                                try {
                                    ramUsage = std::stol(value_str); // Convert VmRSS value to long
                                } catch (const std::invalid_argument& e) {
                                    cerr << "Warning: Invalid VmRSS value for PID " << pid << ": " << value_str << endl;
                                    ramUsage = 0; // Set to 0 on conversion error
                                }
                                break; // Found VmRSS, no need to read further
                            }
                        }


                        processes.push_back({pid, processName, cpuUsagePercent, ramUsage}); // Add ramUsage
                        prevProcessCpuTimes[pid] = {utime, stime};


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

    prevTotalSystemTime = currentTotalSystemTime;
    prevIdleSystemTime = currentIdleSystemTime;


    sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuUsagePercent > b.cpuUsagePercent;
    });

    return processes;
}