#include <climits>

#include "Preemptive.h"

namespace CPUVisualizer
{
    SchedulerResult Preemptive::CalculatePriority(std::vector<Process> inputs)
    {
        SchedulerResult result;
        if (inputs.empty())
            return result;

        result.processes = inputs;

        for (auto& p : result.processes)
            p.remainingTime = p.burstTime;

        int currentTime = 0;
        int completed = 0;
        int n = result.processes.size();

        int minArrival = result.processes[0].arrivalTime;
        for (const auto& p : result.processes)
        {
            if (p.arrivalTime < minArrival)
                minArrival = p.arrivalTime;
        }
        currentTime = minArrival;

        int lastPid = -1;
        int segmentStart = currentTime;

        while (completed < n)
        {
            int highestPriorityIdx = -1;
            int highestPriority = INT_MAX;

            for (int i = 0; i < n; i++)
            {
                if (result.processes[i].arrivalTime <= currentTime && result.processes[i].remainingTime > 0)
                {
                    if (result.processes[i].priority < highestPriority ||
                        (result.processes[i].priority == highestPriority && result.processes[i].arrivalTime <
                            result.processes[highestPriorityIdx].arrivalTime))
                    {
                        highestPriority = result.processes[i].priority;
                        highestPriorityIdx = i;
                    }
                }
            }

            int currentPid = (highestPriorityIdx == -1) ? -1 : result.processes[highestPriorityIdx].id;

            if (currentPid != lastPid)
            {
                if (lastPid != -1)
                {
                    result.scheduleHistory.push_back({ lastPid, segmentStart, currentTime });
                }

                lastPid = currentPid;
                segmentStart = currentTime;
            }

            if (highestPriorityIdx == -1)
            {
                currentTime++;
                continue;
            }

            auto& current = result.processes[highestPriorityIdx];

            if (current.startTime == -1)
            {
                current.startTime = currentTime;
            }
            current.remainingTime--;
            currentTime++;

            if (current.remainingTime == 0)
            {
                completed++;
                current.completionTime = currentTime;
                current.turnaroundTime = current.completionTime - current.arrivalTime;
                current.waitingTime = current.turnaroundTime - current.burstTime;
            }
        }

        if (lastPid != -1)
        {
            result.scheduleHistory.push_back({ lastPid, segmentStart, currentTime });
        }

        float totalWait = 0;
        float totalTAT = 0;

        for (const auto& p : result.processes)
        {
            totalWait += p.waitingTime;
            totalTAT += p.turnaroundTime;
        }

        result.totalTime = currentTime;
        result.averageWaiting = totalWait / n;
        result.averageTurnaround = totalTAT / n;

        std::sort(result.processes.begin(), result.processes.end(), CompareArrival);

        return result;
    }
}