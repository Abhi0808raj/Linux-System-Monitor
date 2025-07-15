#Cmake for compiling the files

cmake_minimum_required(VERSION 3.16)

project(LinuxSystemMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Tell AUTOUIC where to find UI files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)

# Add include directory for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather source files
set(SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/cpu.cpp
        src/memory.cpp
        src/process.cpp
)

# Gather header files (for IDEs, not strictly required for build)
set(HEADERS
        include/mainwindow.h
        include/cpu.h
        include/memory.h
        include/process.h
)

# UI file
set(UIS
        ui/mainwindow.ui
)

qt_add_executable(LinuxSystemMonitor
        ${SOURCES}
        ${HEADERS}
        ${UIS}
)

target_link_libraries(LinuxSystemMonitor PRIVATE Qt6::Widgets)
