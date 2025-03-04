#include "display.h"
#include "cpu.h"
#include "memory.h"
#include "process.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void DisplayMonitor() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    while (true) {
        clear();
        mvprintw(1, 2, "== SYSTEM MONITOR ==");

        float cpuUsage = GetCpuUsage();
        mvprintw(3, 2, "CPU Usage: %.2f%%", cpuUsage);

        auto [totalMemKB, memUtilization] = GetMemoryUsage();
        if (totalMemKB == -1) {
            mvprintw(4, 2, "Memory: Error reading memory info");
        } else {
            double totalMemMB = static_cast<double>(totalMemKB) / 1024.0;
            mvprintw(4, 2, "Memory Usage: %.2f%% (Total: %.2f MB)", memUtilization, totalMemMB);
        }

        int runningProcesses = GetRunningProcesses();
        mvprintw(5, 2, "Running Processes: %d", runningProcesses);

        // Display Process List - Sorted by CPU Usage
        mvprintw(7, 2, "== Processes (Sorted by CPU%) ==");
        mvprintw(8, 2, "PID     CPU%    RAM (MB) NAME"); // Added RAM (MB) column header
        mvprintw(9, 2, "------- ------- -------- --------------------"); // Separator line

        std::vector<ProcessInfo> processes = GetProcessList();
        int startRow = 10;
        int processCount = 0;
        int maxProcessesToDisplay = 10;

        for (const auto& process : processes) {
            if (processCount >= maxProcessesToDisplay) break;
            double ramUsageMB = static_cast<double>(process.ramUsageKB) / 1024.0; // Convert KB to MB for display
            mvprintw(startRow + processCount, 2, "%-7d %-7.1f%%  %-8.1f %-.20s", process.pid, process.cpuUsagePercent, ramUsageMB, process.name.c_str()); // Display RAM and format
            processCount++;
        }

        refresh();
        this_thread::sleep_for(chrono::seconds(1));
    }

    endwin();
}