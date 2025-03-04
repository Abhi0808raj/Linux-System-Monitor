#include "display.h"
#include "cpu.h"
#include "memory.h"
#include "process.h"
#include <ncurses.h>
#include <thread>
#include <chrono>

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

        auto [totalMem, freeMem] = GetMemoryUsage();
        long usedMem = totalMem - freeMem;
        mvprintw(4, 2, "Memory: %ld MB / %ld MB", usedMem / 1024, totalMem / 1024);

        int runningProcesses = GetRunningProcesses();
        mvprintw(5, 2, "Running Processes: %d", runningProcesses);

        refresh();
        this_thread::sleep_for(chrono::seconds(1));
    }

    endwin();
}
