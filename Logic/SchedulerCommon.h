#pragma once

#include <vector>

namespace CPUVisualizer
{
    struct Process
    {
        int id;
        int arrivalTime;
        int burstTime;
        int priority = 0;
        int remainingTime = 0;

        int startTime = -1;
        int completionTime = 0;
        int waitingTime = 0;
        int turnaroundTime = 0;
    };

    struct ProcessInput
    {
        int pid;
        int arrival;
        int burst;
        int priority;
        int memory;
    };

    struct GanttSegment
    {
        int pid;
        int startTime;
        int endTime;
    };

    struct SchedulerResult
    {
        std::vector<Process> processes;
        std::vector<GanttSegment> scheduleHistory;
        float averageWaiting = 0.0f;
        float averageTurnaround = 0.0f;
        int totalTime = 0;
    };

    static bool CompareArrival(const Process& a, const Process& b)
    {
        if (a.arrivalTime != b.arrivalTime)
            return a.arrivalTime < b.arrivalTime;
        return a.id < b.id;
    }
}