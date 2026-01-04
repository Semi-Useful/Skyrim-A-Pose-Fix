#pragma once 
namespace APoseFix 
{
    class Converter 
    {
        public:
        static bool PortHavokFile(const std::filesystem::path& a_inputPath, const std::filesystem::path& a_outputPath);

        private:
        static inline std::filesystem::path dataPath {std::filesystem::current_path() / "Data"};
        static inline std::filesystem::path hkxcPath {dataPath / "hkxc.exe"};
    };
}