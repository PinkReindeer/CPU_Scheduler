#pragma once

#include <vector>
#include <algorithm>

struct Process
{
    int id;
    int arrivalTime;
    int burstTime;

    int startTime = 0;
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
};

struct FCFSResult
{
    std::vector<Process> processes; 
    float averageWaiting = 0.0f;
    float averageTurnaround = 0.0f;
    int totalTime = 0;
};

class FCFS
{
public:
    static FCFSResult Calculate(std::vector<Process> inputs);
};