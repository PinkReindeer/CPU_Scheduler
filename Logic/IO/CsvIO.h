#pragma once

#include <string>

#include "../SchedulerCommon.h"

namespace CPUVisualizer
{
    class CsvIO
    {
    public:
        static std::vector<ProcessInput> ImportProcesses(const std::string& filepath);
        static void ExportResults(const std::string& filepath, const SchedulerResult& results);

        static std::string OpenFileDialog();
        static std::string SaveFileDialog();
    };
}