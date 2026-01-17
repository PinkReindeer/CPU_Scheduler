#include "Application.h"

#include <iostream>

// --- THÊM CÁC THƯ VIỆN NÀY NẾU CHƯA CÓ TRONG PCH ---
#include <glad/glad.h>      // Hoặc loader bạn đang dùng
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
// ----------------------------------------------------

#include "IconsFontAwesome6.h"

namespace CPUVisualizer
{
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        (void)window;
        glViewport(0, 0, width, height);
    }

    Application::Application(const ApplicationConfiguration& spec)
        : m_Specification(spec)
    {
        // Constructor của m_SchedulerUI sẽ tự động được gọi ở đây
    }

    Application::~Application()
    {
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::Run()
    {
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        m_WindowHandle = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.title, nullptr, nullptr);
        if (m_WindowHandle == nullptr)
            return;

        glfwMakeContextCurrent(m_WindowHandle);
        glfwSetFramebufferSizeCallback(m_WindowHandle, FramebufferSizeCallback);
        glfwSwapInterval(1);

        // Load OpenGL functions (GLAD)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("fonts/Poppins-Regular.ttf", 18.0f);
        if (mainFont == nullptr) io.Fonts->AddFontDefault();

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = 18.0f;
        iconsConfig.GlyphOffset = ImVec2(0, 1);

        io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 16.0f, &iconsConfig, icons_ranges);

        ImGui_ImplGlfw_InitForOpenGL(m_WindowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        while (!glfwWindowShouldClose(m_WindowHandle) && m_Running)
        {
            Loop();
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_WindowHandle);
        glfwTerminate();
    }

    void Application::Loop()
    {
        glfwPollEvents();

        // Start Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- GỌI UI TỪ BIẾN THÀNH VIÊN ---
        // (Không dùng static nữa)
        m_SchedulerUI.Render();

        // Rendering
        ImGui::Render();

        int displayWidth, displayHeight;
        glfwGetFramebufferSize(m_WindowHandle, &displayWidth, &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);

        glClearColor(0.06f, 0.09f, 0.13f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_WindowHandle);
    }
}