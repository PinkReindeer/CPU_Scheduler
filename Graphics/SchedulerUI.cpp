#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>

#include "IconsFontAwesome6.h"

#include "../Logic/IO/CsvIO.h"
#include "SchedulerUI.h"
#include "Theme.h"

namespace CPUVisualizer
{
    SchedulerUI::SchedulerUI() {}

    void SchedulerUI::SetupStyles()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ItemSpacing = ImVec2(16, 16);
        style.WindowPadding = ImVec2(32, 32);

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = Theme::Background;
        colors[ImGuiCol_ChildBg] = Theme::Card;
        colors[ImGuiCol_Text] = Theme::TextMain;
        colors[ImGuiCol_Border] = Theme::Border;
        colors[ImGuiCol_Button] = Theme::Accent;
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.55f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.35f, 0.90f, 1.00f);
        colors[ImGuiCol_Header] = Theme::Accent;
    }

    void SchedulerUI::DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly)
    {
        float safeWidth = std::max(10.0f, width);
        ImGui::BeginGroup();

        ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextSecondary);
        ImGui::Text("%s", label);
        ImGui::PopStyleColor();

        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float boxHeight = 45.0f;

        drawList->AddRectFilled(p, ImVec2(p.x + safeWidth, p.y + boxHeight), ImGui::GetColorU32(Theme::InputBg), 6.0f);
        drawList->AddRect(p, ImVec2(p.x + safeWidth, p.y + boxHeight), ImGui::GetColorU32(Theme::Border), 6.0f);

        float textHeight = ImGui::GetTextLineHeight();
        float textCenterY = (boxHeight - textHeight) / 2.0f;

        ImGui::SetCursorScreenPos(ImVec2(p.x + 12, p.y + textCenterY));
        ImGui::TextColored(ImVec4(0.5f, 0.6f, 0.8f, 1.0f), "%s", icon);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

        std::string id = "##" + std::string(label);
        if (readOnly)
        {
            ImGui::SetCursorScreenPos(ImVec2(p.x + 40, p.y + textCenterY));
            ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextMain);
            ImGui::Text("P%d", *value);
            ImGui::PopStyleColor();
        }
        else
        {
            float inputY = textCenterY - ImGui::GetStyle().FramePadding.y;
            ImGui::SetCursorScreenPos(ImVec2(p.x + 40, p.y + inputY));
            ImGui::SetNextItemWidth(safeWidth - 50);
            ImGui::InputInt(id.c_str(), value, 0, 0);
        }
        ImGui::PopStyleColor(2);

        ImGui::SetCursorScreenPos(p);
        ImGui::Dummy(ImVec2(safeWidth, boxHeight));

        ImGui::EndGroup();
    }

    std::vector<Process> SchedulerUI::PrepareLogicInputs() const
    {
        std::vector<Process> logicInputs;
        logicInputs.reserve(m_Processes.size());
        for (const auto& p : m_Processes)
        {
            Process proc;
            proc.id = p.pid;
            proc.arrivalTime = p.arrival;
            proc.burstTime = p.burst;
            proc.priority = p.priority;
            proc.memoryNeeded = p.memory;
            logicInputs.push_back(proc);
        }
        return logicInputs;
    }

    void SchedulerUI::RenderHeader()
    {
        float windowWidth = ImGui::GetContentRegionAvail().x;
        ImGui::TextColored(Theme::Accent, ICON_FA_MICROCHIP "  CPU Scheduler");
        ImGui::SameLine(windowWidth - 150);
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.6f, 1.0f), ICON_FA_CIRCLE " SYSTEM READY");
        ImGui::Spacing();
    }

    void SchedulerUI::RenderIntro()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Setup Simulation");
        ImGui::PopFont();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextColored(Theme::TextSecondary, "Configure your process list below. Choose a scheduling algorithm, input process parameters.");
        ImGui::PopTextWrapPos();
        ImGui::Spacing();
    }

    void SchedulerUI::RenderSetupCard()
    {
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::Border);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));
        
        ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;

        if (ImGui::BeginChild("SetupCard", ImVec2(0, 290), true, flags))
        {
            ImGui::TextColored(Theme::TextSecondary, "SCHEDULING ALGORITHM");

            float availW = ImGui::GetContentRegionAvail().x;

            {
                bool isFCFS = (m_SelectedAlgo == 0);
                if (!isFCFS)
                    ImGui::PushStyleColor(ImGuiCol_Button, Theme::InputBg);
                if (ImGui::Button(ICON_FA_CLOCK "   First Come First Serve (FCFS)", ImVec2((availW * 0.5f) - 5, 40)))
                    m_SelectedAlgo = 0;
                if (!isFCFS)
                    ImGui::PopStyleColor();
            }
            ImGui::SameLine();
            {
                bool isPriority = (m_SelectedAlgo == 1);
                if (!isPriority)
                    ImGui::PushStyleColor(ImGuiCol_Button, Theme::InputBg);
                if (ImGui::Button(ICON_FA_EXCLAMATION "   Priority Scheduling", ImVec2(ImGui::GetContentRegionAvail().x, 40)))
                    m_SelectedAlgo = 1;
                if (!isPriority)
                    ImGui::PopStyleColor();
            }

            int numCols = (m_SelectedAlgo == 1) ? 5 : 4;
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float totalSpacing = (numCols - 1) * spacing;
            float currentAvailW = ImGui::GetContentRegionAvail().x;

            float colW = (currentAvailW > totalSpacing) ? (currentAvailW - totalSpacing) / (float)numCols : 100.0f;

            DrawInputGroup("PROCESS ID", ICON_FA_MICROCHIP, &m_PIDCounter, colW, true);

            ImGui::SameLine();

            DrawInputGroup("ARRIVAL TIME (MS)", ICON_FA_CLOCK, &m_InArrival, colW);
            if (m_InArrival < 0) m_InArrival = 0;

            ImGui::SameLine();

            DrawInputGroup("BURST TIME (MS)", ICON_FA_BOLT, &m_InBurst, colW);
            if (m_InBurst < 1) m_InBurst = 1;

            if (m_InMemory < 0) m_InMemory = 0;

            if (m_SelectedAlgo == 1)
            {
                ImGui::SameLine();
                DrawInputGroup("PRIORITY", ICON_FA_ARROW_DOWN_SHORT_WIDE, &m_InPriority, colW);
                if (m_InPriority < 0) m_InPriority = 0;
            }

            ImGui::SameLine();

            DrawInputGroup("MEMORY (MB)", ICON_FA_MEMORY, &m_InMemory, colW);

            ImGui::TextColored(Theme::TextSecondary, "SYSTEM MEMORY LIMIT (MB)");
            ImGui::SetNextItemWidth(200);
            ImGui::InputInt("##TotalMem", &m_SystemTotalMemory, 128, 1024);
            if (m_SystemTotalMemory < 1) m_SystemTotalMemory = 1;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 65);

            float btnW = 150.0f;
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetStyle().WindowPadding.x - btnW - 24);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.35f, 1.0f));
            if (ImGui::Button(ICON_FA_PLUS " Add Process", ImVec2(btnW, 40)))
            {
                int p = (m_SelectedAlgo == 1) ? m_InPriority : 0;
                m_Processes.push_back({ m_PIDCounter++, m_InArrival, m_InBurst, p, m_InMemory });
                m_InBurst = (rand() % 10) + 1;
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
    }

    void SchedulerUI::RenderProcessQueue()
    {
        ImGui::Text("Process Queue");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Theme::Accent);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::SmallButton(std::to_string(m_Processes.size()).c_str());
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
        ImGui::TextColored(Theme::TextSecondary, "Clear All");
        if (ImGui::IsItemClicked()) {
            m_Processes.clear();
            m_PIDCounter = 1;
        }

        ImGui::Spacing();
    }

    void SchedulerUI::RenderTable()
    {
        float footerHeight = 85.0f;
        float tableHeight = ImGui::GetContentRegionAvail().y - footerHeight;
        if (tableHeight < 100.0f)
            tableHeight = 100.0f;

        ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Card);
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::Border);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);

        if (ImGui::BeginChild("TableScrollRegion", ImVec2(0, tableHeight), true))
        {
            ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_ScrollX;
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(20, 10));

            if (ImGui::BeginTable("table_queue", 6, tableFlags))
            {
                ImGui::TableSetupColumn("PROCESS ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("ARRIVAL TIME", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("BURST TIME", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("MEMORY", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("PRIORITY", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("ACTIONS", ImGuiTableColumnFlags_WidthFixed, 80);

                ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextSecondary);
                ImGui::TableHeadersRow();
                ImGui::PopStyleColor();

                for (int i = 0; i < m_Processes.size(); i++)
                {
                    float rowHeight = 60.0f;
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);

                    float textOffset = (rowHeight - ImGui::GetTextLineHeight()) / 2.0f;
                    float badgeOffset = (rowHeight - 32.0f) / 2.0f;

                    // Col 1: Badge + ID
                    ImGui::TableSetColumnIndex(0);
                    float cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
                    ImGui::SetCursorPosY(cellY + badgeOffset);
                    ImGui::PushStyleColor(ImGuiCol_Button, Theme::Accent);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20);
                    ImGui::Button(("P" + std::to_string(m_Processes[i].pid)).c_str(), ImVec2(32, 32));
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(Theme::TextMain, " P-%d", m_Processes[i].pid);

                    // Col 2: Arrival Time
                    ImGui::TableSetColumnIndex(1);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(Theme::TextSecondary, "%d ms", m_Processes[i].arrival);

                    // Col 3: Burst
                    ImGui::TableSetColumnIndex(2);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(Theme::TextSecondary, "%d ms", m_Processes[i].burst);

                    // Col 4: Memory
                    ImGui::TableSetColumnIndex(3);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    if (m_Processes[i].memory > m_SystemTotalMemory)
                        ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "%d MB (!)", m_Processes[i].memory);
                    else
                        ImGui::TextColored(Theme::TextSecondary, "%d MB", m_Processes[i].memory);

                    // Col 5: Priority
                    ImGui::TableSetColumnIndex(4);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(Theme::TextSecondary, "%d", m_Processes[i].priority);

                    // Col 6: Trash
                    ImGui::TableSetColumnIndex(5);
                    cellY = ImGui::GetCursorPosY();
                    float colW = ImGui::GetContentRegionAvail().x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - 30) / 2);
                    ImGui::SetCursorPosY(cellY + textOffset);

                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0.1f));
                    ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextSecondary);
                    if (ImGui::Button(ICON_FA_TRASH)) {
                        m_Processes.erase(m_Processes.begin() + i);
                        i--;
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }

    void SchedulerUI::RenderFooter()
    {
        ImGui::Separator();

        float btnWidth = 150.0f;
        float regionAvail = ImGui::GetContentRegionAvail().x;

        ImGui::SetCursorPosX(regionAvail - (btnWidth * 3 + 40));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::Border);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        if (ImGui::Button(ICON_FA_FILE_IMPORT "  Import CSV", ImVec2(btnWidth, 45)))
        {
            std::string path = CsvIO::OpenFileDialog();
            if (!path.empty())
            {
                std::vector<ProcessInput> imported = CsvIO::ImportProcesses(path);

                for (auto& p : imported)
                {
                    if (p.arrival < 0) p.arrival = 0;

                    if (p.burst < 1) p.burst = 1;

                    if (p.priority < 0) p.priority = 0;
                }

                if (!imported.empty())
                {
                    m_Processes = imported;
                    int maxPid = 0;
                    for (const auto& p : m_Processes)
                        if (p.pid > maxPid)
                            maxPid = p.pid;
                    m_PIDCounter = maxPid + 1;
                }
            }
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::Border);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        if (ImGui::Button("Reset Default", ImVec2(btnWidth, 45)))
        {
            m_Processes.clear();
            m_PIDCounter = 1;
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Theme::Accent);
        if (ImGui::Button(ICON_FA_PLAY "  Run Simulation", ImVec2(btnWidth + 20, 45)))
        {
            if (!m_Processes.empty())
            {
                auto logicInputs = PrepareLogicInputs();

                auto start = std::chrono::high_resolution_clock::now();

                if (m_SelectedAlgo == 0)
                    m_Results = FCFS::Calculate(logicInputs);
                else if (m_SelectedAlgo == 1)
                    m_Results = Preemptive::CalculatePriority(logicInputs);
                m_ShowResults = true;

                m_Results.systemMemorySize = m_SystemTotalMemory;

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> elapsed = end - start;

                std::cout << "Calculation took: " << elapsed.count() << " ms" << std::endl;
            }
        }
        ImGui::PopStyleColor();
    }

    void SchedulerUI::Render()
    {
        SetupStyles();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGui::Begin("CPU Scheduler", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_HorizontalScrollbar);

        RenderHeader();
        RenderIntro();
        RenderSetupCard();
        ImGui::Spacing();
        RenderProcessQueue();
        RenderTable();
        RenderFooter();

        ImGui::End();

        if (m_ShowResults)
            RenderResults();
    }

    void SchedulerUI::RenderMetrics()
    {
        float contentW = ImGui::GetContentRegionAvail().x;

        ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Card);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);

        if (ImGui::BeginChild("MetricsCard", ImVec2(contentW, 125), true))
        {
            ImGui::Columns(2, "MetricCols", false);

            auto RenderCenteredMetric = [&](const char* label, float value, ImVec4 valColor) {
                    float colW = ImGui::GetColumnWidth();
                    float currentY = ImGui::GetCursorPosY();

                    char valStr[32];
                    snprintf(valStr, sizeof(valStr), "%.2f ms", value);

                    ImVec2 labelSize = ImGui::CalcTextSize(label);
                    ImVec2 valSizeBase = ImGui::CalcTextSize(valStr);
                    ImVec2 valSizeScaled = ImVec2(valSizeBase.x * 1.5f, valSizeBase.y * 1.5f);

                    float contentHeight = labelSize.y + ImGui::GetStyle().ItemSpacing.y + valSizeScaled.y;
                    float boxHeight = ImGui::GetWindowHeight() - ImGui::GetStyle().WindowPadding.y * 2;

                    float offsetY = (boxHeight - contentHeight) / 2.0f;
                    if (offsetY > 0)
                        ImGui::SetCursorPosY(currentY + offsetY);

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - labelSize.x) / 2.0f);
                    ImGui::TextColored(Theme::TextSecondary, "%s", label);

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - valSizeScaled.x) / 2.0f);
                    ImGui::SetWindowFontScale(1.5f);
                    ImGui::TextColored(valColor, "%s", valStr);
                    ImGui::SetWindowFontScale(1.0f);
                };

            RenderCenteredMetric("Avg. Waiting Time", m_Results.averageWaiting, Theme::TextMain);

            ImGui::NextColumn();

            RenderCenteredMetric("Avg. Turnaround Time", m_Results.averageTurnaround, Theme::Accent);

            ImGui::Columns(1);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    void SchedulerUI::RenderGanttChart()
    {
        float contentW = ImGui::GetContentRegionAvail().x;
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Background);
        if (ImGui::BeginChild("GanttArea", ImVec2(contentW, 120), true))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();

            float availW = ImGui::GetContentRegionAvail().x;
            float availH = ImGui::GetContentRegionAvail().y;

            float chartHeight = 50.0f;
            float paddingX = 40.0f;

            float chartY = p.y + (availH - chartHeight) / 2.0f;

            float totalTime = (float)m_Results.totalTime;
            if (totalTime < 1.0f)
                totalTime = 1.0f;

            float safeW = availW - paddingX;
            float unitW = safeW / totalTime;
            float startX = p.x + (paddingX / 2.0f);

            ImU32 blueColor = IM_COL32(60, 120, 200, 255);
            ImU32 whiteTransparent = IM_COL32(255, 255, 255, 100);
            ImU32 textWhite = IM_COL32(255, 255, 255, 255);
            ImU32 textGray = IM_COL32(180, 180, 180, 255);
            ImU32 lineGray = IM_COL32(255, 255, 255, 50);

            for (const auto& segment : m_Results.scheduleHistory)
            {
                float x0 = startX + (segment.startTime * unitW);
                float x1 = startX + (segment.endTime * unitW);
                float y0 = chartY;
                float y1 = chartY + chartHeight;

                if (x1 <= x0) continue;

                draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), blueColor, 4.0f);
                draw_list->AddRect(ImVec2(x0, y0), ImVec2(x1, y1), whiteTransparent, 4.0f);

                std::string label = "P" + std::to_string(segment.pid);
                ImVec2 textSize = ImGui::CalcTextSize(label.c_str());

                if (x1 - x0 > textSize.x + 4)
                {
                    float textX = x0 + ((x1 - x0) - textSize.x) / 2.0f;
                    float textY = y0 + ((chartHeight)-textSize.y) / 2.0f;
                    draw_list->AddText(ImVec2(textX, textY), textWhite, label.c_str());
                }

                std::string tStr = std::to_string(segment.endTime);
                draw_list->AddText(ImVec2(x1 - 5, y1 + 5), textGray, tStr.c_str());
                draw_list->AddLine(ImVec2(x1, y0 - 5), ImVec2(x1, y1 + 5), lineGray);

                if (segment.startTime == 0)
                {
                    draw_list->AddText(ImVec2(x0, y1 + 5), textGray, "0");
                    draw_list->AddLine(ImVec2(x0, y0 - 5), ImVec2(x0, y1 + 5), lineGray);
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void SchedulerUI::RenderMemoryVisualization()
    {
        float contentW = ImGui::GetContentRegionAvail().x;
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Background);
        if (ImGui::BeginChild("MemoryVis", ImVec2(contentW, 120), true))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();
            float availW = ImGui::GetContentRegionAvail().x;
            float barHeight = 40.0f;
            float barY = p.y + 10.0f;

            draw_list->AddRect(ImVec2(p.x, barY), ImVec2(p.x + availW, barY + barHeight),
                IM_COL32(100, 100, 100, 255), 4.0f);

            std::string totalMemStr = "Total: " + std::to_string(m_Results.systemMemorySize) + " MB";
            draw_list->AddText(ImVec2(p.x + availW - 100, barY - 20), IM_COL32(200, 200, 200, 255), totalMemStr.c_str());

            float currentUsagePixels = 0.0f;
            long long currentUsageMB = 0;
            bool overflowShown = false;
            float pixelsPerMB = availW / (float)m_Results.systemMemorySize;

            for (const auto& proc : m_Results.processes)
            {
                if (proc.memoryNeeded <= 0) continue;

                if (proc.memoryNeeded > m_Results.systemMemorySize)
                {
                    float iconX = p.x + 10 + (proc.id * 15);
                    draw_list->AddText(ImVec2(iconX, barY + barHeight + 5), IM_COL32(255, 50, 50, 255), ICON_FA_TRIANGLE_EXCLAMATION);
                    continue;
                }

                if (currentUsageMB + proc.memoryNeeded <= m_Results.systemMemorySize)
                {
                    float width = proc.memoryNeeded * pixelsPerMB;
                    float startX = p.x + currentUsagePixels;
                    float endX = startX + width;

                    draw_list->AddRectFilled(ImVec2(startX, barY), ImVec2(endX, barY + barHeight),
                        IM_COL32(60, 180, 100, 200), 0.0f);

                    draw_list->AddRect(ImVec2(startX, barY), ImVec2(endX, barY + barHeight),
                        IM_COL32(0, 0, 0, 100));

                    if (width > 20)
                    {
                        std::string label = "P" + std::to_string(proc.id);
                        ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
                        draw_list->AddText(ImVec2(startX + (width - textSize.x) / 2, barY + (barHeight - textSize.y) / 2),
                            IM_COL32(255, 255, 255, 255), label.c_str());
                    }

                    currentUsagePixels += width;
                    currentUsageMB += proc.memoryNeeded;
                }
                else
                {
                    if (!overflowShown)
                    {
                        float limitX = p.x + currentUsagePixels;

                        draw_list->AddLine(ImVec2(limitX, barY - 10), ImVec2(limitX, barY + barHeight + 10),
                            IM_COL32(255, 50, 50, 255), 3.0f);

                        std::string overflowText = "OVERFLOW";
                        ImVec2 textSize = ImGui::CalcTextSize(overflowText.c_str());

                        float textX = p.x + availW - textSize.x + 20.0f;

                        float textY = barY - 38.0f;

                        draw_list->AddText(ImVec2(textX, textY), IM_COL32(255, 100, 100, 255), overflowText.c_str());

                        overflowShown = true;
                    }
                }
            }

            std::string usageStr = "Used: " + std::to_string(currentUsageMB) + " MB";
            draw_list->AddText(ImVec2(p.x, barY - 20), IM_COL32(100, 255, 100, 255), usageStr.c_str());
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void SchedulerUI::RenderDetailedAnalysis()
    {
        ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;

        if (ImGui::BeginTable("ResultTable", 6, tableFlags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50);
            ImGui::TableSetupColumn("Arrival Time");
            ImGui::TableSetupColumn("Burst Time");
            ImGui::TableSetupColumn("Finish Time");
            ImGui::TableSetupColumn("Turnaround");
            ImGui::TableSetupColumn("Waiting Time");
            ImGui::TableHeadersRow();

            for (const auto& p : m_Results.processes)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored(Theme::Accent, "P%d", p.id);

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", p.arrivalTime);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", p.burstTime);

                ImGui::TableSetColumnIndex(3);
                ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "%d", p.completionTime);

                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%d", p.turnaroundTime);
                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%d", p.waitingTime);
            }
            ImGui::EndTable();
        }
    }

    void SchedulerUI::RenderResults()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 winSize(viewport->Size.x * 0.85f, viewport->Size.y * 0.85f);
        ImGui::SetNextWindowSize(winSize, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::Begin("Simulation Results", &m_ShowResults, ImGuiWindowFlags_NoCollapse);

        float contentW = ImGui::GetContentRegionAvail().x;

        ImGui::TextColored(Theme::Accent, ICON_FA_CHART_PIE " Performance Metrics");

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 140);
        if (ImGui::Button(ICON_FA_FILE_EXPORT "  Export CSV", ImVec2(140, 30)))
        {
            std::string path = CsvIO::SaveFileDialog();
            if (!path.empty())
            {
                CsvIO::ExportResults(path, m_Results);
            }
        }

        RenderMetrics();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::CollapsingHeader(ICON_FA_CHART_BAR " Gantt Chart Visualization", ImGuiTreeNodeFlags_Framed))
        {
            RenderGanttChart();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::CollapsingHeader(ICON_FA_MEMORY " Main Memory Allocation", ImGuiTreeNodeFlags_Framed))
        {
            RenderMemoryVisualization();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(Theme::Accent, ICON_FA_LIST " Detailed Analysis");
        RenderDetailedAnalysis();

        ImGui::End();
    }
}