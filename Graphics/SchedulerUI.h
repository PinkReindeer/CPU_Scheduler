#pragma once

#include <vector>
#include <string>
#include <imgui.h>
#include "../Logic/FCFS.h" // <--- THÊM DÒNG NÀY để nhận diện struct Process

namespace CPUVisualizer
{
    // (Đã xóa struct Process ở đây vì nó đã nằm bên FCFS.h rồi)

    class SchedulerUI
    {
    public:
        SchedulerUI();
        ~SchedulerUI() = default;

        void Render();

    private:
        void SetupStyles();
        void DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly = false);
        void RenderSetup();
        void RenderResults();

        // Bây giờ nó sẽ hiểu Process nhờ include FCFS.h bên trên
        std::vector<Process> m_Processes;

        int m_PIDCounter = 1;
        int m_SelectedAlgo = 0;
        bool m_ShowResults = false;

        int m_InArrival = 0;
        int m_InBurst = 4;
        int m_InPriority = 2;
    };
}