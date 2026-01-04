#include "hook.h"
#include "RE/A/AnimationClipDataSingleton.h"
#include "RE/H/hkbBehaviorReferenceGenerator.h"
#include "converter.h"

namespace APoseFix
{
    RE::hkbBehaviorGraph *LoadBehaviorGraphHook::Load(const char *a_folderRoot, const char *a_behaviorFilePath, uint64_t a_assetLoader, void *a_unk04)
    {
        auto result =  _Load(a_folderRoot, a_behaviorFilePath, a_assetLoader, a_unk04);
        
        if (!result && a_folderRoot && a_behaviorFilePath && a_folderRoot[0] != '\0' && a_behaviorFilePath[0] != '\0')
        {
            std::string convertedFileName = std::format("{}_packed", a_behaviorFilePath);
            std::filesystem::path dataPath = std::filesystem::current_path() / "Data";
            std::filesystem::path hkxcPath = dataPath / "hkxc.exe";
            std::filesystem::path path = dataPath / "Meshes" / a_folderRoot / a_behaviorFilePath;
            std::filesystem::path convertedPath{ path }; 
            convertedPath.replace_filename(std::format("{}_packed.hkx", path.stem().string()));
            if (std::filesystem::exists(path))
            {
                if (Converter::PortHavokFile(path, convertedPath))
                {
                    std::error_code removeErrorCode;
                    // std::filesystem::remove(path, removeErrorCode);
                    std::error_code renameErrorCode;
                    std::filesystem::copy(convertedPath, path, std::filesystem::copy_options::overwrite_existing, renameErrorCode);
                    if (removeErrorCode.value() == 0 && renameErrorCode.value() == 0)
                    {
                        std::filesystem::path behaviorPath(a_behaviorFilePath);
                        std::string convertedBehaviorFileName = std::format("{}\\{}_packed.hkx",
                                                                            behaviorPath.parent_path().string(),
                                                                            behaviorPath.stem().string());
                        result = _Load(a_folderRoot, convertedBehaviorFileName.c_str(), a_assetLoader, a_unk04);
                        if (result)
                        {
                            SKSE::log::info("Behavior Graph {}\\{} loaded after conversion", a_folderRoot, convertedFileName);
                            return result;
                        }
                        SKSE::log::error("Behavior Graph {}\\{} still not loadable after conversion", a_folderRoot, a_behaviorFilePath);
                    }
                    else
                    {
                        SKSE::log::error("Failed to rename converted behavior graph from {} to {}: {}", convertedPath.string(), path.string(), renameErrorCode.message());
                        SKSE::log::error("Or failed to remove original behavior graph {}: {}", path.string(), removeErrorCode.message());
                    }
                }
                else
                {
                    SKSE::log::error("Failed to convert behavior graph {} to {}", path.string(), convertedPath.string());
                }


            
            }
            SKSE::log::info("Behavior Graph {}\\{} not loadable, redirecting to dummy behavior", a_folderRoot, a_behaviorFilePath);
            a_folderRoot = "Actors\\Character";
            a_behaviorFilePath = "Behaviors\\dummybehavior.hkx";
            SKSE::log::info("Redirected to {}\\{}", a_folderRoot, a_behaviorFilePath);
            return _Load(a_folderRoot, a_behaviorFilePath, a_assetLoader, a_unk04);
        }
        // SKSE::log::info("Behavior Graph {}\\{} loaded successfully", a_folderRoot, a_behaviorFilePath);
        return result;
    }

    bool CombinePathsHook::CombinePaths(const char *a_path1, const char *a_path2, const char* unk03)
    {
        if (a_path1 && a_path2 && a_path1[0] != '\0' && a_path2[0] != '\0')
        {
                SKSE::log::info("Combining Paths: {} + {}", a_path1, a_path2);
        }
        return _CombinePaths(a_path1, a_path2, unk03);
    }
    int32_t ClipLoadHook::Queue(RE::AnimationFileManagerSingleton *a_manager, RE::hkbContext &a_context, RE::hkbClipGenerator *a_clipGenerator, RE::BSSynchronizedClipGenerator *a_syncClipGen)
    {

        auto result = _Queue(a_manager, a_context, a_clipGenerator, a_syncClipGen);
        if (a_clipGenerator->userData == 0xC)
        {
            SKSE::log::info("Clip generator {} loaded 2", a_clipGenerator->animationName.c_str());
        }
        else 
        {
            SKSE::log::info("Clip generator {} not loaded", a_clipGenerator->animationName.c_str());
            
        }
        return result;
    }
    bool ClipLoadHook::Load(RE::AnimationFileManagerSingleton *a_manager, const RE::hkbContext &a_context, RE::hkbClipGenerator *a_clipGenerator, RE::BSSynchronizedClipGenerator *a_synchronizedClipGenerator)
    {
        if (a_clipGenerator->userData == 0xC)
        {
            SKSE::log::info("Clip generator {} loaded 3", a_clipGenerator->animationName.c_str());
        }
        auto result =  _Load(a_manager, a_context, a_clipGenerator, a_synchronizedClipGenerator);
        auto bindingIndex = a_clipGenerator->animationBindingIndex;
        if (bindingIndex < 0)
        {
            return result;
        }
        auto* character = a_context.character;
        if (!character)
        {
            return result;
        }
        const RE::BShkbAnimationGraph* graph = stl::adjust_pointer<RE::BShkbAnimationGraph>(a_context.character, -0xC0);
        if (!graph)
        {
            return result;
        }
        auto setup = character->setup;
        if (!setup)
        {
            return result;
        }
        auto data = setup->data;
        if (!data)
        {
            return result;
        }
        auto stringData = data->stringData;
        if (!stringData)
        {
            return result;
        }
        auto &animationBundleNames = stringData->animationNames;
        auto* animationClipDataSingleton = RE::AnimationClipDataSingleton::GetSingleton();
        if (!animationClipDataSingleton)
        {

            return result;
        }

        auto* animationData = animationClipDataSingleton->anim_datas.find(graph->projectName)->second.get();
        if (!animationData)
        {
            return result;
        }
        for(auto& animationFile : animationData->hkxFiles)
        {
            SKSE::log::info("Animation file in project {}: {}", graph->projectName.c_str(), animationFile.c_str());
        }
        if (animationBundleNames.size() > bindingIndex)
        {
            auto *str = animationBundleNames[bindingIndex].data();
            SKSE::log::info("Clip generator {} loaded 4", str);
        }
        return result;
    }
}
