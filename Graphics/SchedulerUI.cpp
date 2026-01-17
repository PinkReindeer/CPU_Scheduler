#include "SchedulerUI.h"
#include <algorithm> // std::max
#include <iostream>
#include <string>

// Header icon font (đảm bảo bạn đã có file này)
#include "IconsFontAwesome6.h" 

// --- COLOR PALETTE ---
#define COL_BG          ImVec4(0.05f, 0.07f, 0.12f, 1.00f)
#define COL_CARD        ImVec4(0.08f, 0.10f, 0.16f, 1.00f)
#define COL_INPUT_BG    ImVec4(0.11f, 0.14f, 0.22f, 1.00f)
#define COL_BORDER      ImVec4(0.20f, 0.25f, 0.35f, 1.00f)
#define COL_ACCENT      ImVec4(0.15f, 0.45f, 1.00f, 1.00f)
#define COL_TEXT_SEC    ImVec4(0.55f, 0.60f, 0.70f, 1.00f)
#define COL_TEXT_MAIN   ImVec4(0.95f, 0.96f, 0.98f, 1.00f)
#define COL_SUCCESS     ImVec4(0.2f, 0.8f, 0.6f, 1.0f)
#define COL_WARNING     ImVec4(1.0f, 0.7f, 0.2f, 1.0f)

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

    // --- HÀM CHÍNH ĐỂ ĐIỀU HƯỚNG ---
    void SchedulerUI::Render()
    {
        SetupStyles();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGui::Begin("CPU Scheduler", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);


// Header chung
// (Dùng màu xanh dương trực tiếp, bỏ icon Microchip thay bằng chữ thường)
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "CPU Scheduler Visualizer");

        ImGui::SameLine(ImGui::GetWindowWidth() - 160);

        if (m_ShowResults)
        {
            // (Dùng màu Xanh Lá trực tiếp, thay icon bằng chữ [Done])
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.6f, 1.0f), "[Done] SIMULATION DONE");
        }
        else
        {
            // (Dùng màu Cam trực tiếp, thay icon bằng chữ [Ready])
            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "[Ready] SYSTEM READY");
        }

        ImGui::Spacing(); ImGui::Spacing();

        // -----------------------------

        // Điều hướng: Nếu cờ showResult = true thì vẽ kết quả, ngược lại vẽ setup
        if (m_ShowResults) {
            RenderResults();
        }
        else {
            RenderSetup();
        }

        ImGui::End();
    }

    // --- MÀN HÌNH NHẬP LIỆU (Setup) ---
    void SchedulerUI::RenderSetup()
    {
        // --- INTRO ---
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Setup Simulation");
        ImGui::PopFont();
        ImGui::TextColored(COL_TEXT_SEC, "Configure your process list below. Choose a scheduling algorithm, input process parameters.");
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
                m_InBurst = (rand() % 10) + 1; // Random burst time cho lần nhập sau đỡ phải gõ
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

        // --- TABLE INPUT ---
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

                    // Col 1: ID
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

                    // Col 2: Arrival
                    ImGui::TableSetColumnIndex(1);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].arrival);

                    // Col 3: Burst
                    ImGui::TableSetColumnIndex(2);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d ms", m_Processes[i].burst);

                    // Col 4: Priority
                    ImGui::TableSetColumnIndex(3);
                    cellY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + moveRight);
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::TextColored(COL_TEXT_SEC, "%d", m_Processes[i].priority);

                    // Col 5: Actions
                    ImGui::TableSetColumnIndex(4);
                    cellY = ImGui::GetCursorPosY();
                    float columnWidth = ImGui::GetContentRegionAvail().x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - 30) / 2.0f);
                    ImGui::SetCursorPosY(cellY + textOffset);
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT_SEC);
                    if (ImGui::Button(ICON_FA_TRASH)) {
                        m_Processes.erase(m_Processes.begin() + i);
                        i--;
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        // --- FOOTER (Run Button) ---
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

        // NÚT RUN: KÍCH HOẠT LOGIC TẠI ĐÂY
        if (ImGui::Button(ICON_FA_PLAY "  Run Simulation", ImVec2(btnWidth + 20, 45)))
        {
            if (!m_Processes.empty())
            {
                if (m_SelectedAlgo == 0) // FCFS
                {
                    FCFS::RunSimulation(m_Processes);
                }
                // Nếu sau này thêm Priority thì else if (m_SelectedAlgo == 1) ...

                m_ShowResults = true; // Chuyển sang màn hình kết quả
            }
        }
        ImGui::PopStyleColor();
    }

    // --- MÀN HÌNH KẾT QUẢ (Results) ---
    void SchedulerUI::RenderResults()
    {
        // Nút quay lại
        if (ImGui::Button(ICON_FA_ARROW_LEFT " Back to Setup")) {
            m_ShowResults = false;
        }

        ImGui::SameLine();
        ImGui::Text("Simulation Results (Algorithm: %s)", m_SelectedAlgo == 0 ? "FCFS" : "Priority");

        ImGui::Spacing(); ImGui::Spacing();

        // --- GANTT CHART ---
        ImGui::TextColored(COL_TEXT_SEC, "GANTT CHART");
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_CARD);
        if (ImGui::BeginChild("GanttChart", ImVec2(0, 100), true))
        {
            if (m_Processes.empty()) { ImGui::EndChild(); ImGui::PopStyleColor(); return; }

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();
            float availW = ImGui::GetContentRegionAvail().x;
            float totalTime = (float)m_Processes.back().completionTime;
            float height = 40.0f;
            float startY = p.y + 20.0f;

            if (totalTime > 0)
            {
                for (const auto& proc : m_Processes)
                {
                    float x0 = p.x + (proc.startTime / totalTime) * availW;
                    float x1 = p.x + (proc.completionTime / totalTime) * availW;
                    float width = x1 - x0;

                    // Vẽ Block
                    drawList->AddRectFilled(ImVec2(x0, startY), ImVec2(x1, startY + height), ImGui::GetColorU32(COL_ACCENT));
                    drawList->AddRect(ImVec2(x0, startY), ImVec2(x1, startY + height), ImGui::GetColorU32(COL_BG), 2.0f);

                    // Vẽ PID
                    std::string idText = "P" + std::to_string(proc.pid);
                    // Chỉ vẽ text nếu block đủ rộng
                    if (width > 20) {
                        ImVec2 txtSz = ImGui::CalcTextSize(idText.c_str());
                        drawList->AddText(ImVec2(x0 + (width - txtSz.x) / 2, startY + (height - txtSz.y) / 2), IM_COL32_WHITE, idText.c_str());
                    }

                    // Vẽ mốc thời gian (bắt đầu và kết thúc)
                    std::string tStart = std::to_string(proc.startTime);
                    std::string tEnd = std::to_string(proc.completionTime);

                    drawList->AddText(ImVec2(x0, startY + height + 5), IM_COL32(150, 150, 150, 255), tStart.c_str());
                    // Chỉ vẽ số cuối cùng của process cuối, các số giữa đã có start của thằng sau đè lên rồi
                    if (proc.pid == m_Processes.back().pid) {
                        drawList->AddText(ImVec2(x1 - 5, startY + height + 5), IM_COL32(150, 150, 150, 255), tEnd.c_str());
                    }
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Spacing(); ImGui::Spacing();

        // --- RESULT TABLE ---
        ImGui::TextColored(COL_TEXT_SEC, "DETAILED METRICS");
        ImGui::PushStyleColor(ImGuiCol_ChildBg, COL_CARD);
        if (ImGui::BeginChild("ResultTable", ImVec2(0, 0), true))
        {
            ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable("table_results", 6, tableFlags))
            {
                ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 50);
                ImGui::TableSetupColumn("Arrival");
                ImGui::TableSetupColumn("Burst");
                ImGui::TableSetupColumn("Finish Time");
                ImGui::TableSetupColumn("Turnaround Time");
                ImGui::TableSetupColumn("Waiting Time");

                ImGui::TableHeadersRow();

                double totalWait = 0, totalTurn = 0;

                for (const auto& p : m_Processes)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0); ImGui::Text("P%d", p.pid);
                    ImGui::TableSetColumnIndex(1); ImGui::Text("%d", p.arrival);
                    ImGui::TableSetColumnIndex(2); ImGui::Text("%d", p.burst);
                    ImGui::TableSetColumnIndex(3); ImGui::TextColored(COL_SUCCESS, "%d", p.completionTime);
                    ImGui::TableSetColumnIndex(4); ImGui::Text("%d", p.turnaroundTime);
                    ImGui::TableSetColumnIndex(5); ImGui::Text("%d", p.waitingTime);

                    totalWait += p.waitingTime;
                    totalTurn += p.turnaroundTime;
                }

                // Row Average
                if (!m_Processes.empty())
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(COL_ACCENT, "AVG");

                    ImGui::TableSetColumnIndex(4);
                    ImGui::TextColored(COL_ACCENT, "%.2f", totalTurn / m_Processes.size());

                    ImGui::TableSetColumnIndex(5);
                    ImGui::TextColored(COL_ACCENT, "%.2f", totalWait / m_Processes.size());
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
}