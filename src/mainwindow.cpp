// Implementation for main window
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cpu.h"
#include "memory.h"
#include "process.h"

#include <QTimer>
#include <QString>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up process table (assuming you have a QTableWidget named processTable)
    ui->processTable->setColumnCount(4);
    ui->processTable->setHorizontalHeaderLabels({"PID", "CPU %", "RAM (MB)", "Name"});

    // Set up timer to update stats every second
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateStats);
    timer->start(1000);

    updateStats(); // Initial update
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStats()
{
    // Update CPU usage label (assuming you have a QLabel named cpuLabel)
    float cpuUsage = GetCpuUsage();
    ui->cpuLabel->setText(QString("CPU Usage: %1%").arg(cpuUsage, 0, 'f', 2));

    // Update Memory usage label (assuming you have a QLabel named memoryLabel)
    auto [totalMemKB, memUtilization] = GetMemoryUsage();
    double totalMemMB = totalMemKB / 1024.0;
    ui->memoryLabel->setText(
        QString("Memory Usage: %1% (Total: %2 MB)")
            .arg(memUtilization, 0, 'f', 2)
            .arg(totalMemMB, 0, 'f', 2)
    );

    // Update running processes label (assuming you have a QLabel named processCountLabel)
    int runningProcesses = GetRunningProcesses();
    ui->processCountLabel->setText(QString("Running Processes: %1").arg(runningProcesses));

    // Update process table
    auto processes = GetProcessList();
    int maxRows = std::min(10, static_cast<int>(processes.size()));
    ui->processTable->setRowCount(maxRows);

    for (int i = 0; i < maxRows; ++i) {
        const auto& proc = processes[i];
        ui->processTable->setItem(i, 0, new QTableWidgetItem(QString::number(proc.pid)));
        ui->processTable->setItem(i, 1, new QTableWidgetItem(QString::number(proc.cpuUsagePercent, 'f', 1)));
        ui->processTable->setItem(i, 2, new QTableWidgetItem(QString::number(proc.ramUsageKB / 1024.0, 'f', 1)));
        ui->processTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(proc.name)));
    }
}
