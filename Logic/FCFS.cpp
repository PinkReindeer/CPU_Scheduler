// File: Logic/FCFS.cpp
#include "FCFS.h"
#include <algorithm> // Cần cho std::sort
#include <fstream>   // Cần cho ghi file
#include <iostream>  // Cần cho debug

namespace CPUVisualizer {

    bool FCFS::compareArrival(const Process& a, const Process& b) {
        if (a.arrival == b.arrival)
            return a.pid < b.pid;
        return a.arrival < b.arrival;
    }

    void FCFS::RunSimulation(std::vector<Process>& processes) {
        if (processes.empty()) return;

        // 1. Sắp xếp theo Arrival Time
        std::sort(processes.begin(), processes.end(), compareArrival);

        int currentTime = 0;

        for (auto& p : processes) {
            // Nếu CPU rảnh (thời gian hiện tại nhỏ hơn thời gian đến của process)
            if (currentTime < p.arrival) {
                currentTime = p.arrival;
            }

            // 2. Tính toán
            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burst;
            p.turnaroundTime = p.completionTime - p.arrival;
            p.waitingTime = p.turnaroundTime - p.burst;
            p.responseTime = p.waitingTime; // Với FCFS thì Response = Waiting

            // Cập nhật thời gian hiện tại
            currentTime = p.completionTime;
        }
    }

    void FCFS::ExportToCSV(const std::vector<Process>& processes, const std::string& filePath) {
        std::ofstream file(filePath);
        if (!file.is_open()) return;

        // Ghi Header
        file << "PID,Arrival,Burst,Priority,Start,Completion,Waiting,Turnaround\n";

        double totalWait = 0;
        double totalTurn = 0;

        for (const auto& p : processes) {
            file << p.pid << ","
                << p.arrival << ","
                << p.burst << ","
                << p.priority << ","
                << p.startTime << ","
                << p.completionTime << ","
                << p.waitingTime << ","
                << p.turnaroundTime << "\n";

            totalWait += p.waitingTime;
            totalTurn += p.turnaroundTime;
        }

        // Ghi dòng trung bình (Average)
        if (!processes.empty()) {
            file << ",,,AVERAGE,,,"
                << (totalWait / processes.size()) << ","
                << (totalTurn / processes.size()) << "\n";
        }

        file.close();
    }
}