#include <Windows.h>
#include <commdlg.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "CsvIO.h"

namespace CPUVisualizer
{
    std::vector<ProcessInput> CsvIO::ImportProcesses(const std::string& filepath)
    {
        std::vector<ProcessInput> processes;
        std::ifstream file(filepath);

        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return processes;
        }

        std::string line;
        bool firstLine = true;

        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            if (firstLine)
            {
                firstLine = false;
                if (!isdigit(line[0]))
                    continue;
            }

            std::stringstream ss(line);
            std::string segment;
            std::vector<std::string> seglist;

            while (std::getline(ss, segment, ','))
            {
                seglist.push_back(segment);
            }

            if (seglist.size() >= 3)
            {
                ProcessInput p;
                try
                {
                    p.pid = std::stoi(seglist[0]);
                    p.arrival = std::stoi(seglist[1]);
                    p.burst = std::stoi(seglist[2]);
                    p.priority = (seglist.size() > 3) ? std::stoi(seglist[3]) : 0;
                    processes.push_back(p);
                }
                catch (...)
                {
                    continue;
                }
            }
        }
        return processes;
    }

    void CsvIO::ExportResults(const std::string& filepath, const SchedulerResult& results)
    {
        std::ofstream file(filepath);
        if (!file.is_open())
            return;

        file << "ID,Arrival Time,Burst Time,Finish Time,Turnaround Time,Waiting Time\n";

        for (const auto& p : results.processes)
        {
            file << p.id << "," << p.arrivalTime << "," << p.burstTime << ","
                << p.completionTime << "," << p.turnaroundTime << "," << p.waitingTime
                << "\n";
        }

        file << "\n";
        file << "Average Turnaround Time," << results.averageTurnaround << "\n";
        file << "Average Waiting Time," << results.averageWaiting << "\n";
    }

    std::string CsvIO::OpenFileDialog()
    {
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return std::string(ofn.lpstrFile);
        }
        return "";
    }

    std::string CsvIO::SaveFileDialog()
    {
        OPENFILENAMEA ofn;
        char szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "CSV Files\0*.csv\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            std::string filename = ofn.lpstrFile;
            if (filename.find(".csv") == std::string::npos)
                filename += ".csv";
            return filename;
        }
        return "";
    }
}