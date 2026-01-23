#include "FCFS.h"

namespace CPUVisualizer
{
    SchedulerResult FCFS::Calculate(std::vector<Process> inputs)
    {
        SchedulerResult result;
        if (inputs.empty()) return result;

        result.processes = inputs;
        std::sort(result.processes.begin(), result.processes.end(), CompareArrival);

        int currentTime = 0;
        float totalWait = 0;
        float totalTAT = 0;

        for (auto& p : result.processes)
        {
            if (currentTime < p.arrivalTime)
            {
                currentTime = p.arrivalTime;
            }

            p.startTime = currentTime;
            p.completionTime = p.startTime + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.burstTime;

            result.scheduleHistory.push_back({ p.id, p.startTime, p.completionTime });

            totalWait += p.waitingTime;
            totalTAT += p.turnaroundTime;

            currentTime = p.completionTime;
        }

        result.totalTime = currentTime;
        result.averageWaiting = totalWait / result.processes.size();
        result.averageTurnaround = totalTAT / result.processes.size();

        return result;
    }
}