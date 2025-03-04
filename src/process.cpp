#include "process.h"
#include <fstream>
#include <string>

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
