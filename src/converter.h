#pragma once 

namespace APoseFix 
{
    class Converter 
    {
        public:
        static bool PortHavokFile(const std::filesystem::path& a_inputPath); 
        static bool PortHavokFile(const std::filesystem::path& a_inputPath, const std::filesystem::path& a_outputPath);
        static bool HasErrors() { return !errorPaths.empty(); }
        static void ClearErrors() { errorPaths.clear(); }
        static std::string GetErrorReport();
        private:
        static inline std::filesystem::path dataPath {std::filesystem::current_path() / "Data"};
        static inline std::vector<std::filesystem::path> errorPaths; 
    };
}