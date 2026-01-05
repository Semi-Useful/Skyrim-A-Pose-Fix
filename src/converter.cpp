#include "converter.h"

namespace APoseFix
{
    bool Converter::PortHavokFile(const std::filesystem::path &a_inputPath)
    {
        std::filesystem::path convertedPath{a_inputPath};
        convertedPath.replace_filename(std::format("{}_cv.hkx", a_inputPath.stem().string()));
        if (!PortHavokFile(a_inputPath, convertedPath))
        {
            return false;
        }
        std::error_code removeErrorCode;
        std::filesystem::remove(a_inputPath, removeErrorCode);
        if (removeErrorCode.value() != 0)
        {
            return false;
        }
        std::error_code renameErrorCode;
        std::filesystem::rename(convertedPath, a_inputPath, renameErrorCode);
        if (renameErrorCode.value() != 0)
        {
            return false;
        }
        return true;
    }

    bool Converter::PortHavokFile(const std::filesystem::path &a_inputPath, const std::filesystem::path &a_outputPath)
    {
        if (!std::filesystem::exists(hkxcPath))
        {
            return false;
        }
        std::lock_guard<std::mutex> guard(converterLock);
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
        WaitForSingleObject(pi.hProcess, 5000);
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (exit_code != EXIT_SUCCESS)
        {
            return false;
        }
        // auto message = std::format("APoseFix: Ported {} to 64 bit", a_inputPath.filename().string());
        // RE::DebugNotification(message.c_str());
        return std::filesystem::exists(a_outputPath);
    }
}
