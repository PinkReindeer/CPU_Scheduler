#pragma once

#include <vector>
#include <string>
#include <imgui.h>

namespace CPUVisualizer
{
    struct UIProcess
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
        ~SchedulerUI() = default;

        void Render();

    private:
        void DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly = false);
        void SetupStyles();

        std::vector<UIProcess> m_Processes;
        int m_PIDCounter = 1;
        int m_SelectedAlgo = 1; // 0: FCFS, 1: Priority

        // Biến Input tạm thời
        int m_InArrival = 0;
        int m_InBurst = 4;
        int m_InPriority = 2;
    };
}