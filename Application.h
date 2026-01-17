#pragma once
#include "Graphics/SchedulerUI.h" // <--- 1. Đảm bảo đường dẫn đúng tới file SchedulerUI.h

namespace CPUVisualizer
{
    struct ApplicationConfiguration
    {
        int width = 1280;
        int height = 720;
        const char* title = "CPU Schedule Demonstration";
    };

    class Application
    {
    public:
        Application(const ApplicationConfiguration& spec = ApplicationConfiguration());
        ~Application();

        void Run();
        void Close();

    private:
        void Loop();

    private:
        ApplicationConfiguration m_Specification;
        struct GLFWwindow* m_WindowHandle = nullptr;
        bool m_Running = true;

        // --- 2. KHAI BÁO UI LÀ THÀNH VIÊN CỦA CLASS ---
        SchedulerUI m_SchedulerUI;
    };
}