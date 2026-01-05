#include "converter.h"
#include <serde_hkx_ffi.h>
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
        if (std::filesystem::exists(a_outputPath))
        {
            std::error_code removeErrorCode;
            std::filesystem::remove(a_outputPath, removeErrorCode);
        }
        auto pathStr = a_inputPath.string();
        auto convertedPathStr = a_outputPath.string();
        auto serdeResult = serde_hkx_ffi_convert(pathStr.c_str(), convertedPathStr.c_str(), OutputFormat::SERDE_HKX_AMD64);
        if (serdeResult != SerdeHkxError::SERDE_HKX_OK)
        {
            return false;
        }
        return std::filesystem::exists(a_outputPath);
    }
}
