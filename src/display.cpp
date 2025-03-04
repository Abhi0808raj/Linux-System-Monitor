#include "display.h"
#include "cpu.h"
#include "memory.h"
#include "process.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip> // For fixed and setprecision

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

        auto [totalMemKB, memUtilization] = GetMemoryUsage(); // Get totalMemKB and utilization
        if (totalMemKB == -1) {
            mvprintw(4, 2, "Memory: Error reading memory info");
        } else {
            double totalMemMB = static_cast<double>(totalMemKB) / 1024.0;
            mvprintw(4, 2, "Memory Usage: %.2f%% (Total: %.2f MB)", memUtilization, totalMemMB); // Display utilization and total memory
        }

        int runningProcesses = GetRunningProcesses();
        mvprintw(5, 2, "Running Processes: %d", runningProcesses);

        refresh();
        this_thread::sleep_for(chrono::seconds(1));
    }

    endwin();
}