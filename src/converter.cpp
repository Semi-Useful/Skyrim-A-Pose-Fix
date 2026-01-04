#include "converter.h"

namespace APoseFix
{
    bool Converter::PortHavokFile(const std::filesystem::path &a_inputPath, const std::filesystem::path &a_outputPath)
    {
        if (!std::filesystem::exists(hkxcPath))
        {
            return false;
        }
        if (std::filesystem::exists(a_outputPath))
        {
            std::error_code removeErrorCode;
            std::filesystem::remove(a_outputPath, removeErrorCode);
        }
        auto hkxcPathStr = hkxcPath.string();
        auto pathStr = a_inputPath.string();
        auto convertedPathStr = a_outputPath.string();
        auto command = std::format("\"{}\" convert -i \"{}\" -o \"{}\" -v amd64", hkxcPathStr, pathStr, convertedPathStr);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess(hkxcPathStr.c_str(),
                command.data(),
                nullptr,
                nullptr,
                FALSE,
                CREATE_NO_WINDOW,
                nullptr,
                nullptr,
                &si, &pi))
        {
            return false;
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (exit_code != EXIT_SUCCESS)
        {
            return false;
        }
        return std::filesystem::exists(a_outputPath);
    }
}
