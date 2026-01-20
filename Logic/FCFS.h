#pragma once

#include <algorithm>

#include "SchedulerCommon.h"

namespace CPUVisualizer
{
    struct FCFS
    {
        static SchedulerResult Calculate(std::vector<Process> inputs);
    };
}