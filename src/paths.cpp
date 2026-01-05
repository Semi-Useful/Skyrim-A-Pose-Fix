#include "paths.h"

namespace APoseFix
{
    void PathResolver::RegisterParentPath(const std::string_view& a_projectName, const std::string_view& a_folderName)
    {
        WriteLocker locker(dataLock);
        projectFolderMap.emplace(a_projectName, a_folderName);
        folderProjectMap.emplace(a_folderName, a_projectName);  
    }
    bool PathResolver::GetRelativeFolderPath(const std::string_view &a_projectName, std::filesystem::path &a_outFolderPath)
    {
        ReadLocker locker(dataLock);
        auto it = projectFolderMap.find(a_projectName);
        if (it != projectFolderMap.end())
        {
            a_outFolderPath = it->second;
            return true;
        }
        return false;
    }
    bool PathResolver::GetAbsoluteFolderPath(const std::string_view &a_projectName, const std::string_view &a_folderName, std::filesystem::path &a_outFolderPath)
    {
        ReadLocker locker(dataLock);
        auto it = projectFolderMap.find(a_projectName);
        
        if (it != projectFolderMap.end())
        {
            std::filesystem::path path = std::filesystem::path(a_folderName).lexically_normal();
            auto topDirectory = path.begin();
            if (topDirectory == path.end())
            {
                return false;
            }

            for(const auto& component : path)
            {
                if (component == "OpenAnimationReplacer")
                {
                    return false;
                }
            }

            if (*topDirectory == "data")
            {
                a_outFolderPath = std::filesystem::current_path() / a_folderName;
                return true;
            }
            if (*topDirectory == "meshes")
            {
                a_outFolderPath = std::filesystem::current_path() / "Data" / a_folderName;
                return true;
            }
            if (*topDirectory == it->second)
            {
                a_outFolderPath = std::filesystem::current_path() / "Data" / "Meshes" / path;
                return true;
            }
            a_outFolderPath = std::filesystem::current_path() / "Data" / "Meshes"/ it->second / a_folderName;
            return true;
        }
        return false;
    }
}