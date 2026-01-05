#include "converter.h"
#include <serde_hkx_ffi.h>
#include <future>
#include <chrono>
#include <thread>
#include <atomic>
#include <Windows.h>

namespace APoseFix
{
    constexpr auto CONVERT_TIMEOUT = std::chrono::seconds(2);
    
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
        if (std::filesystem::exists(a_outputPath))
        {
            std::error_code removeErrorCode;
            std::filesystem::remove(a_outputPath, removeErrorCode);
        }
        auto pathStr = a_inputPath.string();
        auto convertedPathStr = a_outputPath.string();
        

        auto promise = std::make_shared<std::promise<SerdeHkxError>>();
        auto future = promise->get_future();
        
        std::thread([promise, pathStr, convertedPathStr]() {
            auto result = serde_hkx_ffi_convert(pathStr.c_str(), convertedPathStr.c_str(), OutputFormat::SERDE_HKX_AMD64);
            promise->set_value(result);
        }).detach();
        
        auto status = future.wait_for(CONVERT_TIMEOUT);
        if (status == std::future_status::timeout)
        {
            SKSE::log::error("Conversion timed out for file: {}", pathStr);
            // std::error_code removeErrorCode;
            // std::filesystem::remove(a_inputPath, removeErrorCode);
            // if (removeErrorCode.value() != 0)
            // {
            //     SKSE::log::error("Failed to remove input file after timeout: {}: {}", pathStr, removeErrorCode.message());
            // }
            errorPaths.push_back(a_inputPath);
            return false; 
        }
        
        auto serdeResult = future.get();
        if (serdeResult != SerdeHkxError::SERDE_HKX_OK)
        {
            return false;
        }
        return std::filesystem::exists(a_outputPath);
    }
    std::string Converter::GetErrorReport()
    {
        std::string errorPathStr;
        for(const auto& path : Converter::errorPaths)
        {
            errorPathStr += path.relative_path().string() + ",\n";
        }
        return std::format("APoseFix\nThe following Havok files are corrupt and must be fixed manually: {}", errorPathStr);
    }
}
