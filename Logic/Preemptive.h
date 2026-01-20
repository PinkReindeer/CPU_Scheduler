#pragma once

#include <algorithm>

#include "SchedulerCommon.h"

namespace CPUVisualizer
{
	struct Preemptive
	{
		static SchedulerResult CalculatePriority(std::vector<Process> inputs);
	};
}