#include "memory.h"
#include <fstream>
#include <string>

using namespace std;

pair<long, long> GetMemoryUsage() {
    ifstream file("/proc/meminfo");
    string key;
    long totalMem = 0, freeMem = 0, buffers = 0, cached = 0;

    while (file >> key) {
        long value;
        file >> value;
        
        if (key == "MemTotal:") totalMem = value;
        else if (key == "MemFree:") freeMem = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;
    }

    long usedMem = totalMem - (freeMem + buffers + cached);

    return {totalMem, usedMem};
}
