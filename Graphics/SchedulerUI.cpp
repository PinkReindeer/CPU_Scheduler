#include "SchedulerUI.h"
#include <algorithm> // std::max
#include <iostream>
#include <string>

#include "IconsFontAwesome6.h" 

// --- COLOR PALETTE ---
#define COL_BG          ImVec4(0.05f, 0.07f, 0.12f, 1.00f)
#define COL_CARD        ImVec4(0.08f, 0.10f, 0.16f, 1.00f)
#define COL_INPUT_BG    ImVec4(0.11f, 0.14f, 0.22f, 1.00f)
#define COL_BORDER      ImVec4(0.20f, 0.25f, 0.35f, 1.00f)
#define COL_ACCENT      ImVec4(0.15f, 0.45f, 1.00f, 1.00f)
#define COL_TEXT_SEC    ImVec4(0.55f, 0.60f, 0.70f, 1.00f)
#define COL_TEXT_MAIN   ImVec4(0.95f, 0.96f, 0.98f, 1.00f)

namespace CPUVisualizer
{
    SchedulerUI::SchedulerUI()
    {
    }

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

        ImGui::Begin("CPU Scheduler", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        // --- HEADER ---
        ImGui::TextColored(COL_ACCENT, ICON_FA_MICROCHIP "  CPU Scheduler");
        ImGui::SameLine(ImGui::GetWindowWidth() - 160);
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.6f, 1.0f), ICON_FA_CIRCLE " SYSTEM READY");
        ImGui::Spacing(); ImGui::Spacing();

        // --- INTRO ---
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Setup Simulation");
        ImGui::PopFont();
        ImGui::TextColored(COL_TEXT_SEC, "Configure your process list below. Choose a scheduling algorithm, input process\nparameters, and run the simulation.");
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

                if (ImGui::Button(ICON_FA_CLOCK "   First Come First Serve (FCFS)", ImVec2(availW * 0.49f, 45)))
                    m_SelectedAlgo = 0;

                if (!isFCFS) ImGui::PopStyleColor();
            }

            ImGui::SameLine();

            // Tương tự cho nút Priority
            {
                bool isPriority = (m_SelectedAlgo == 1);
                if (!isPriority) ImGui::PushStyleColor(ImGuiCol_Button, COL_INPUT_BG);

                if (ImGui::Button(ICON_FA_EXCLAMATION "   Priority Scheduling", ImVec2(availW * 0.49f, 45)))
                    m_SelectedAlgo = 1;

                if (!isPriority) ImGui::PopStyleColor();
            }

            ImGui::Spacing(); ImGui::Spacing();

            // Input Fields
            int numCols = (m_SelectedAlgo == 1) ? 4 : 3;
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float totalSpacing = (numCols - 1) * spacing;
            float colW = (availW > totalSpacing) ? (availW - totalSpacing) / (float)numCols : 50.0f;

            DrawInputGroup("PROCESS ID", ICON_FA_MICROCHIP, &m_PIDCounter, colW, true);
            ImGui::SameLine();
            DrawInputGroup("ARRIVAL TIME (MS)", ICON_FA_CLOCK, &m_InArrival, colW);
            ImGui::SameLine();
            DrawInputGroup("BURST TIME (MS)", ICON_FA_BOLT, &m_InBurst, colW);

            if (m_SelectedAlgo == 1)
            {
                ImGui::SameLine();
                DrawInputGroup("PRIORITY (1=High)", ICON_FA_ARROW_DOWN_SHORT_WIDE, &m_InPriority, colW);
            }

            // Add Button
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 170);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.35f, 1.0f));
            if (ImGui::Button(ICON_FA_PLUS " Add Process", ImVec2(150, 40)))
            {
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

        ImGui::SameLine(ImGui::GetWindowWidth() - 80);
        ImGui::TextColored(COL_TEXT_SEC, "Clear All");
        if (ImGui::IsItemClicked()) { m_Processes.clear(); m_PIDCounter = 1; }

        ImGui::Spacing();

        // --- TABLE ---
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_CARD);
        ImGui::PushStyleColor(ImGuiCol_Border, COL_BORDER);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);

        if (ImGui::BeginChild("TableList", ImVec2(0, 400), true))
        {
            ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_PadOuterX;
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(20, 10));

            if (ImGui::BeginTable("table_queue", 5, tableFlags))
            {
                ImGui::TableSetupColumn("PROCESS ID", ImGuiTableColumnFlags_WidthFixed, 180);
                ImGui::TableSetupColumn("ARRIVAL TIME");
                ImGui::TableSetupColumn("BURST TIME");
                ImGui::TableSetupColumn("PRIORITY");
                ImGui::TableSetupColumn("ACTIONS", ImGuiTableColumnFlags_WidthFixed, 100);

                ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);
                ImGui::TableHeadersRow();
                ImGui::PopStyleColor();

                for (int i = 0; i < m_Processes.size(); i++)
                {
                    float rowHeight = 60.0f;
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);

                    float textOffset = (rowHeight - ImGui::GetTextLineHeight()) / 2.0f;
                    float badgeOffset = (rowHeight - 32.0f) / 2.0f;

                    float moveRight = 15.0f;

                    // --- Cột 1: Badge + ID ---
                    ImGui::TableSetColumnIndex(0);
                    float cellY = ImGui::GetCursorPosY();

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);

                    ImGui::SetCursorPosY(cellY + badgeOffset);
                    ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20);
                    ImGui::Button(("P" + std::to_string(m_Processes[i].pid)).c_str(), ImVec2(32, 32));
                    ImGui::PopStyleVar(); ImGui::PopStyleColor();

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_MAIN, "Process %d", m_Processes[i].pid);

                    // --- Cột 2: Arrival Time ---
                    ImGui::TableSetColumnIndex(1);
                    cellY = ImGui::GetCursorPosY();

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);

                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].arrival);

                    // --- Cột 3: Burst Time ---
                    ImGui::TableSetColumnIndex(2);
                    cellY = ImGui::GetCursorPosY();

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);

                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].burst);

                    // --- Cột 4: Priority ---
                    ImGui::TableSetColumnIndex(3);
                    cellY = ImGui::GetCursorPosY();

                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);

                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d", m_Processes[i].priority);

                    // --- Cột 5: Trash (Actions) ---
                    ImGui::TableSetColumnIndex(4);
                    cellY = ImGui::GetCursorPosY();

                    float columnWidth = ImGui::GetContentRegionAvail().x;
                    float iconWidth = 30.0f;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - iconWidth - 40) / 2.0f);

                    ImGui::SetCursorPosY(cellY + textOffset);

                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0.1f));
                    ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);

                    if (ImGui::Button(ICON_FA_TRASH))
                    {
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
        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        float btnWidth = 150.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (btnWidth * 2 + 50));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, COL_BORDER);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        if (ImGui::Button("Reset Default", ImVec2(btnWidth, 45))) { m_Processes.clear(); m_PIDCounter = 1; }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, COL_ACCENT);
        ImGui::Button(ICON_FA_PLAY "  Run Simulation", ImVec2(btnWidth + 20, 45));
        ImGui::PopStyleColor();

        ImGui::End();
    }
}