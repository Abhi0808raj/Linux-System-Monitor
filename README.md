# Linux System Monitor

A real-time system monitoring application built with Qt6 and C++ that provides comprehensive system information including CPU usage, memory utilization, and running processes.

## Features

- **Real-time CPU Usage**: Monitors CPU utilization percentage with continuous updates
- **Memory Monitoring**: Displays total memory and current utilization percentage
- **Process Management**: Shows running processes count and detailed process information
- **Process Table**: Lists top 10 processes with PID, CPU usage, RAM usage, and process name
- **Auto-refresh**: Updates all statistics every second automatically
- **Cross-platform UI**: Built with Qt6 for a modern, responsive interface

## Screenshots

The application displays:
- System title and overview
- CPU usage percentage
- Memory usage with total memory information
- Running processes count
- Sortable process table showing the most CPU-intensive processes

## Requirements

### System Requirements
- Linux operating system (uses `/proc` filesystem)
- Qt6 Widgets library
- CMake 3.16 or higher
- C++17 compatible compiler

### Dependencies
- Qt6 (Widgets component)
- Standard C++ libraries

## Installation

### Prerequisites
Install Qt6 development packages:

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential

# Fedora/RHEL
sudo dnf install qt6-qtbase-devel qt6-qttools-devel cmake gcc-c++

# Arch Linux
sudo pacman -S qt6-base qt6-tools cmake gcc
```

### Building from Source

1. Clone or download the project
2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
make -j$(nproc)
```

5. Run the application:
```bash
./LinuxSystemMonitor
```

## Project Structure

```
project-root/
├── CMakeLists.txt          # CMake configuration
├── include/                # Header files
│   ├── cpu.h              # CPU monitoring functions
│   ├── memory.h           # Memory monitoring functions
│   ├── process.h          # Process monitoring functions
│   └── mainwindow.h       # Main window class
├── src/                   # Source files
│   ├── main.cpp           # Application entry point
│   ├── mainwindow.cpp     # Main window implementation
│   ├── cpu.cpp            # CPU monitoring implementation
│   ├── memory.cpp         # Memory monitoring implementation
│   └── process.cpp        # Process monitoring implementation
└── ui/                    # UI definition files
    └── mainwindow.ui      # Main window UI layout
```

## Architecture

The application follows a modular design:

### Core Components

1. **CPU Monitor** (`cpu.h/cpp`): Reads from `/proc/stat` to calculate CPU usage
2. **Memory Monitor** (`memory.h/cpp`): Reads from `/proc/meminfo` for memory statistics
3. **Process Monitor** (`process.h/cpp`): Scans `/proc` directory for process information
4. **Main Window** (`mainwindow.h/cpp`): Qt-based GUI that displays all information

### Key Functions

- `GetCpuUsage()`: Returns current CPU utilization percentage
- `GetMemoryUsage()`: Returns total memory and utilization percentage
- `GetRunningProcesses()`: Returns count of currently running processes
- `GetProcessList()`: Returns detailed information about all processes

## Usage

1. Launch the application
2. The interface will automatically start displaying real-time system information
3. The process table shows the top 10 processes sorted by CPU usage
4. All statistics update automatically every second

## Technical Details

### CPU Monitoring
- Reads CPU statistics from `/proc/stat`
- Calculates usage percentage using idle time differences
- Maintains state between readings for accurate calculations

### Memory Monitoring
- Parses `/proc/meminfo` for total and available memory
- Calculates utilization as `(Total - Available) / Total * 100`
- Returns both total memory in KB and utilization percentage

### Process Monitoring
- Scans `/proc` directory for process information
- Reads process names from `/proc/[pid]/comm`
- Calculates CPU usage per process using `/proc/[pid]/stat`
- Reads memory usage from `/proc/[pid]/status` (VmRSS field)
- Sorts processes by CPU usage for display

## Customization

### Refresh Rate
To change the update interval, modify the timer value in `mainwindow.cpp`:
```cpp
timer->start(1000); // Change 1000 (1 second) to desired milliseconds
```

### Process Table Size
To display more or fewer processes, modify the `maxRows` calculation in `updateStats()`:
```cpp
int maxRows = std::min(20, static_cast<int>(processes.size())); // Show 20 instead of 10
```

## Troubleshooting

### Common Issues

1. **Application won't start**: Ensure Qt6 libraries are installed
2. **No data displayed**: Check if `/proc` filesystem is accessible
3. **Build errors**: Verify CMake version and Qt6 development packages

### Debug Mode
To enable debug output, add debug statements or compile with debugging symbols:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on Linux systems
5. Submit a pull request

## License

This project is provided as-is for educational and personal use.

## Future Enhancements

- Network monitoring
- Disk I/O statistics
- System temperature monitoring
- Process kill functionality
- Export statistics to file
- Dark/light theme support
- System tray integration

## Version History

- **v1.0.0**: Initial release with basic CPU, memory, and process monitoring