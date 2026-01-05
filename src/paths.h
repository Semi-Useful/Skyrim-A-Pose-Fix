#pragma once 
#include <shared_mutex>
namespace APoseFix
{
    class PathResolver
    {
        public:
        static void RegisterParentPath(const std::string_view& a_projectName, const std::string_view& a_folderName);
        static bool GetRelativeFolderPath(const std::string_view& a_projectName, std::filesystem::path& a_outFolderPath);
        static bool GetAbsoluteFolderPath(const std::string_view& a_projectName, const std::string_view& a_folderName, std::filesystem::path& a_outFolderPath);
        private:
        using Lock = std::shared_mutex;
        using ReadLocker = std::shared_lock<Lock>;
        using WriteLocker = std::unique_lock<Lock>;

        static inline Lock dataLock; 

        static inline std::unordered_map<std::string_view, std::string> projectFolderMap; 
        static inline std::unordered_map<std::string, std::string_view> folderProjectMap; 
    };
}