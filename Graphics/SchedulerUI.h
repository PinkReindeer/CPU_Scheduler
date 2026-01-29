#pragma once

#include "../Logic/FCFS.h"
#include "../Logic/Preemptive.h"
#include "../Logic/SchedulerCommon.h"

namespace CPUVisualizer
{

    class SchedulerUI
    {
    public:
        SchedulerUI();
        void Render();

    private:
        void SetupStyles();

        void RenderHeader();
        void RenderIntro();
        void RenderSetupCard();
        void RenderProcessQueue();
        void RenderTable();
        void RenderFooter();

        void RenderResults();
        void RenderMetrics();
        void RenderGanttChart();
        void RenderMemoryVisualization();
        void RenderDetailedAnalysis();

        void DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly = false);
        std::vector<Process> PrepareLogicInputs() const;

        int m_SelectedAlgo = 0;
        int m_PIDCounter = 1;
        int m_SystemTotalMemory = 4096;

        int m_InArrival = 0;
        int m_InBurst = 1;
        int m_InPriority = 1;
        int m_InMemory = 0;

        std::vector<ProcessInput> m_Processes;

    private:
        SchedulerResult m_Results;
        bool m_ShowResults = false;
    };
}