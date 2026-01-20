#include <algorithm>
#include <iostream>
#include <string>

#include "IconsFontAwesome6.h"

#include "SchedulerUI.h"

// --- COLOR PALETTE ---
#define COL_BG ImVec4(0.05f, 0.07f, 0.12f, 1.00f)
#define COL_CARD ImVec4(0.08f, 0.10f, 0.16f, 1.00f)
#define COL_INPUT_BG ImVec4(0.11f, 0.14f, 0.22f, 1.00f)
#define COL_BORDER ImVec4(0.20f, 0.25f, 0.35f, 1.00f)
#define COL_ACCENT ImVec4(0.15f, 0.45f, 1.00f, 1.00f)
#define COL_TEXT_SEC ImVec4(0.55f, 0.60f, 0.70f, 1.00f)
#define COL_TEXT_MAIN ImVec4(0.95f, 0.96f, 0.98f, 1.00f)

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
        colors[ImGuiCol_WindowBg] = COL_BG;
        colors[ImGuiCol_ChildBg] = COL_CARD;
        colors[ImGuiCol_Text] = COL_TEXT_MAIN;
        colors[ImGuiCol_Border] = COL_BORDER;
        colors[ImGuiCol_Button] = COL_ACCENT;
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.55f, 1.00f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.35f, 0.90f, 1.00f);
        colors[ImGuiCol_Header] = COL_ACCENT;
    }

    void SchedulerUI::DrawInputGroup(const char* label, const char* icon, int* value, float width, bool readOnly)
    {
        float safeWidth = std::max(10.0f, width);
        ImGui::BeginGroup();

        ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);
        ImGui::Text("%s", label);
        ImGui::PopStyleColor();

        ImGui::Spacing();

        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        float boxHeight = 45.0f;

        ImGui::Dummy(ImVec2(safeWidth, boxHeight));

        drawList->AddRectFilled(p, ImVec2(p.x + safeWidth, p.y + boxHeight), ImGui::GetColorU32(COL_INPUT_BG), 6.0f);
        drawList->AddRect(p, ImVec2(p.x + safeWidth, p.y + boxHeight), ImGui::GetColorU32(COL_BORDER), 6.0f);

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
            ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_MAIN);
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

        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + boxHeight + 10.0f));
        ImGui::EndGroup();
    }

    void SchedulerUI::Render()
    {
        SetupStyles();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGui::Begin("CPU Scheduler", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_HorizontalScrollbar);

        // --- HEADER ---
        float windowWidth = ImGui::GetContentRegionAvail().x;

        ImGui::TextColored(COL_ACCENT, ICON_FA_MICROCHIP "  CPU Scheduler");
        ImGui::SameLine(windowWidth - 150);
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.6f, 1.0f), ICON_FA_CIRCLE " SYSTEM READY");
        ImGui::Spacing();
        ImGui::Spacing();

        // --- INTRO ---
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Setup Simulation");
        ImGui::PopFont();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextColored(COL_TEXT_SEC, "Configure your process list below. Choose a scheduling algorithm, input process parameters.");
        ImGui::PopTextWrapPos();
        ImGui::Spacing();

        // --- SETUP CARD ---
        ImGui::PushStyleColor(ImGuiCol_Border, COL_BORDER);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(24, 24));

        if (ImGui::BeginChild("SetupCard", ImVec2(0, 335), true, ImGuiWindowFlags_AlwaysUseWindowPadding))
        {
            ImGui::TextColored(COL_TEXT_SEC, "SCHEDULING ALGORITHM");

            float availW = ImGui::GetContentRegionAvail().x;

            {
                bool isFCFS = (m_SelectedAlgo == 0);
                if (!isFCFS) ImGui::PushStyleColor(ImGuiCol_Button, COL_INPUT_BG);
                if (ImGui::Button(ICON_FA_CLOCK "   First Come First Serve (FCFS)", ImVec2((availW * 0.5f) - 5, 45))) m_SelectedAlgo = 0;
                if (!isFCFS) ImGui::PopStyleColor();
            }
            ImGui::SameLine();
            {
                bool isPriority = (m_SelectedAlgo == 1);
                if (!isPriority) ImGui::PushStyleColor(ImGuiCol_Button, COL_INPUT_BG);
                if (ImGui::Button(ICON_FA_EXCLAMATION "   Priority Scheduling", ImVec2(ImGui::GetContentRegionAvail().x, 45))) m_SelectedAlgo = 1;
                if (!isPriority) ImGui::PopStyleColor();
            }

            ImGui::Spacing(); ImGui::Spacing();

            int numCols = (m_SelectedAlgo == 1) ? 4 : 3;
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float totalSpacing = (numCols - 1) * spacing;
            float currentAvailW = ImGui::GetContentRegionAvail().x;

            float colW = (currentAvailW > totalSpacing) ? (currentAvailW - totalSpacing) / (float)numCols : 100.0f;

            DrawInputGroup("PROCESS ID", ICON_FA_MICROCHIP, &m_PIDCounter, colW, true);
            ImGui::SameLine();
            DrawInputGroup("ARRIVAL TIME (MS)", ICON_FA_CLOCK, &m_InArrival, colW);
            ImGui::SameLine();
            DrawInputGroup("BURST TIME (MS)", ICON_FA_BOLT, &m_InBurst, colW);
            if (m_SelectedAlgo == 1) {
                ImGui::SameLine();
                DrawInputGroup("PRIORITY", ICON_FA_ARROW_DOWN_SHORT_WIDE, &m_InPriority, colW);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);

            float btnW = 150.0f;
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + ImGui::GetStyle().WindowPadding.x - btnW - 24);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.35f, 1.0f));
            if (ImGui::Button(ICON_FA_PLUS " Add Process", ImVec2(btnW, 40))) {
                int p = (m_SelectedAlgo == 1) ? m_InPriority : 0;
                m_Processes.push_back({ m_PIDCounter++, m_InArrival, m_InBurst, p });
                m_InBurst = (rand() % 10) + 1;
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();

        ImGui::Spacing(); ImGui::Spacing();

        // --- PROCESS QUEUE ---
        ImGui::Text("Process Queue");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::SmallButton(std::to_string(m_Processes.size()).c_str());
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
        ImGui::TextColored(COL_TEXT_SEC, "Clear All");
        if (ImGui::IsItemClicked()) {
            m_Processes.clear();
            m_PIDCounter = 1;
        }

        ImGui::Spacing();

		// --- TABLE ---
        float footerHeight = 85.0f;
        float tableHeight = ImGui::GetContentRegionAvail().y - footerHeight;
        if (tableHeight < 100.0f) tableHeight = 100.0f;

        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_CARD);
        ImGui::PushStyleColor(ImGuiCol_Border, COL_BORDER);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);

        if (ImGui::BeginChild("TableScrollRegion", ImVec2(0, tableHeight), true))
        {
            ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_ScrollX;
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(20, 10));

            if (ImGui::BeginTable("table_queue", 5, tableFlags))
            {
                ImGui::TableSetupColumn("PROCESS ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("ARRIVAL TIME", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("BURST TIME", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("PRIORITY", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("ACTIONS", ImGuiTableColumnFlags_WidthFixed, 80);

                ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);
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
                    ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20);
                    ImGui::Button(("P" + std::to_string(m_Processes[i].pid)).c_str(), ImVec2(32, 32));
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_MAIN, " P-%d", m_Processes[i].pid);

                    // Col 2: Arrival Time
                    ImGui::TableSetColumnIndex(1);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].arrival);

                    // Col 3: Burst
                    ImGui::TableSetColumnIndex(2);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].burst);

                    // Col 4: Priority
                    ImGui::TableSetColumnIndex(3);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d", m_Processes[i].priority);

                    // Col 5: Trash
                    ImGui::TableSetColumnIndex(4);
                    cellY = ImGui::GetCursorPosY();
                    float colW = ImGui::GetContentRegionAvail().x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - 30) / 2);
                    ImGui::SetCursorPosY(cellY + textOffset);

                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0.1f));
                    ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);
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

        // --- FOOTER ---
        ImGui::Separator();

        float btnWidth = 150.0f;
        float regionAvail = ImGui::GetContentRegionAvail().x;

        ImGui::SetCursorPosX(regionAvail - (btnWidth * 2 + 20));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, COL_BORDER);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        if (ImGui::Button("Reset Default", ImVec2(btnWidth, 45)))
        {
            m_Processes.clear();
            m_PIDCounter = 1;
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
        if (ImGui::Button(ICON_FA_PLAY "  Run Simulation", ImVec2(btnWidth + 20, 45)))
        {
            if (!m_Processes.empty())
            {
                std::vector<Process> logicInputs;
                for (auto& p : m_Processes) {
                    Process proc;
                    proc.id = p.pid;
                    proc.arrivalTime = p.arrival;
                    proc.burstTime = p.burst;
                    proc.priority = p.priority;
                    logicInputs.push_back(proc);
                }
                if (m_SelectedAlgo == 0) m_Results = FCFS::Calculate(logicInputs);
                else if (m_SelectedAlgo == 1) m_Results = Preemptive::CalculatePriority(logicInputs);
                m_ShowResults = true;
            }
        }
        ImGui::PopStyleColor();

        ImGui::End();

        if (m_ShowResults) RenderResults();
    }

    void SchedulerUI::RenderResults()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 winSize(viewport->Size.x * 0.85f, viewport->Size.y * 0.85f);
        ImGui::SetNextWindowSize(winSize, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        ImGui::Begin("Simulation Results", &m_ShowResults, ImGuiWindowFlags_NoCollapse);

        float contentW = ImGui::GetContentRegionAvail().x;

        ImGui::TextColored(COL_ACCENT, ICON_FA_CHART_PIE " Performance Metrics");

        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_CARD);
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
                if (offsetY > 0) ImGui::SetCursorPosY(currentY + offsetY);

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - labelSize.x) / 2.0f);
                ImGui::TextColored(COL_TEXT_SEC, "%s", label);

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (colW - valSizeScaled.x) / 2.0f);
                ImGui::SetWindowFontScale(1.5f);
                ImGui::TextColored(valColor, "%s", valStr);
                ImGui::SetWindowFontScale(1.0f);
                };

            RenderCenteredMetric("Avg. Waiting Time", m_Results.averageWaiting, COL_TEXT_MAIN);

            ImGui::NextColumn();

            RenderCenteredMetric("Avg. Turnaround Time", m_Results.averageTurnaround, COL_ACCENT);

            ImGui::Columns(1);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(COL_ACCENT, ICON_FA_CHART_BAR " Gantt Chart Visualization");

        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_BG);
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
            if (totalTime < 1.0f) totalTime = 1.0f;

            float safeW = availW - paddingX;
            float unitW = safeW / totalTime;
            float startX = p.x + (paddingX / 2.0f);

            ImU32 blueColor = IM_COL32(60, 120, 200, 255);
            ImU32 whiteTransparent = IM_COL32(255, 255, 255, 100);
            ImU32 textWhite = IM_COL32(255, 255, 255, 255);
            ImU32 textGray = IM_COL32(180, 180, 180, 255);
            ImU32 lineGray = IM_COL32(255, 255, 255, 50);

            for (int i = 0; i < m_Results.processes.size(); ++i)
            {
                const auto& proc = m_Results.processes[i];

                float x0 = startX + (proc.startTime * unitW);
                float x1 = startX + (proc.completionTime * unitW);
                float y0 = chartY;
                float y1 = chartY + chartHeight;

                draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), blueColor, 4.0f);
                draw_list->AddRect(ImVec2(x0, y0), ImVec2(x1, y1), whiteTransparent, 4.0f);

                std::string label = "P" + std::to_string(proc.id);
                ImVec2 textSize = ImGui::CalcTextSize(label.c_str());

                if (x1 - x0 > textSize.x + 4)
                {
                    float textX = x0 + ((x1 - x0) - textSize.x) / 2.0f;
                    float textY = y0 + ((chartHeight)-textSize.y) / 2.0f;
                    draw_list->AddText(ImVec2(textX, textY), textWhite, label.c_str());
                }

                std::string tStr = std::to_string(proc.completionTime);
                draw_list->AddText(ImVec2(x1 - 5, y1 + 5), textGray, tStr.c_str());
                draw_list->AddLine(ImVec2(x1, y0 - 5), ImVec2(x1, y1 + 5), lineGray);

                if (proc.startTime == 0)
                {
                    draw_list->AddText(ImVec2(x0, y1 + 5), textGray, "0");
                    draw_list->AddLine(ImVec2(x0, y0 - 5), ImVec2(x0, y1 + 5), lineGray);
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextColored(COL_ACCENT, ICON_FA_LIST " Detailed Analysis");

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
                ImGui::TextColored(COL_ACCENT, "P%d", p.id);

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

        ImGui::End();
    }
}