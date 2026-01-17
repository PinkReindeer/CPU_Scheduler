#include "FCFS.h"

bool CompareArrival(const Process& a, const Process& b) {
    if (a.arrivalTime != b.arrivalTime)
        return a.arrivalTime < b.arrivalTime;
    return a.id < b.id;
}

FCFSResult FCFS::Calculate(std::vector<Process> inputs) {
    FCFSResult result;
    if (inputs.empty()) return result;

    result.processes = inputs;
    std::sort(result.processes.begin(), result.processes.end(), CompareArrival);

    int currentTime = 0;
    float totalWait = 0;
    float totalTAT = 0;

    for (auto& p : result.processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }

        p.startTime = currentTime;
        p.completionTime = p.startTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        totalWait += p.waitingTime;
        totalTAT += p.turnaroundTime;

        currentTime = p.completionTime;
    }

    result.totalTime = currentTime;
    result.averageWaiting = totalWait / result.processes.size();
    result.averageTurnaround = totalTAT / result.processes.size();

    return result;
}