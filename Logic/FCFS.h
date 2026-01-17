#pragma once
#include <vector>
#include <string>

namespace CPUVisualizer {

    // --- ĐỊNH NGHĨA STRUCT TẠI ĐÂY ---
    struct Process {
        int pid;
        int arrival;
        int burst;
        int priority;

        // Output fields
        int startTime = 0;
        int completionTime = 0;
        int waitingTime = 0;
        int turnaroundTime = 0;
        int responseTime = 0;
    };
    // ---------------------------------

    class FCFS {
    public:
        static void RunSimulation(std::vector<Process>& processes);
        static void ExportToCSV(const std::vector<Process>& processes, const std::string& filePath);

    private:
        static bool compareArrival(const Process& a, const Process& b);
    };
}