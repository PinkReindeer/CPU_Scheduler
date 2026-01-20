#pragma once

#include <vector>

#include "../Logic/SchedulerCommon.h"
#include "../Logic/FCFS.h"
#include "../Logic/Preemptive.h"

namespace CPUVisualizer
{
	struct ProcessInput
	{
		int pid;
		int arrival;
		int burst;
		int priority;
	};

	class SchedulerUI
	{
	public:
		SchedulerUI();
		void Render();

	private:
		void SetupStyles();
		void RenderResults();
		void DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly = false);

		int m_SelectedAlgo = 0;
		int m_PIDCounter = 1;

		int m_InArrival = 0;
		int m_InBurst = 1;
		int m_InPriority = 1;

		std::vector<ProcessInput> m_Processes;

	private:
		SchedulerResult m_Results;
		bool m_ShowResults = false;
	};
}