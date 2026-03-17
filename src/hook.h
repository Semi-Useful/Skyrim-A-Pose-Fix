#pragma once
#include <unordered_set>
#include "RE/H/hkbBehaviorReferenceGenerator.h"

namespace APoseFix 
{
    class LoadBehaviorGraphHook
    {
        public:
        static bool Install()
        {
            __try {
                REL::Relocation<std::uintptr_t> target{REL::RelocationID(63030, 63948), REL::Relocate(0x11B, 0x10B)};
                auto &trampoline = SKSE::GetTrampoline();
                _Load = trampoline.write_call<5>(target.address(), Load);
                return true;
            }
            __except(EXCEPTION_EXECUTE_HANDLER) {
                return false;
            }
        }
        private:
        static RE::hkbBehaviorGraph* Load(const char *a_folderRoot, const char *a_behaviorFilePath, uint64_t a_assetLoader, void* a_unk04);
        static inline REL::Relocation<decltype(Load)> _Load;
    };

    class LoadBehaviorProjectHook
    {
        public:
        static bool Install()
        {
            __try {
                REL::Relocation<std::uintptr_t> target{ REL::RelocationID(62640, 63585), REL::Relocate(0x108, 0x111) };
                auto &trampoline = SKSE::GetTrampoline();
                _LoadBehaviorProject = trampoline.write_call<5>(target.address(), LoadBehaviorProject);
                return true;
            }
            __except(EXCEPTION_EXECUTE_HANDLER) {
                return false;
            }
        }
        private:
        static RE::hkbBehaviorGraph* LoadBehaviorProject(const char* folder, const char* name, RE::hkbCharacter& hchar, uint64_t project_data_handle, RE::BSFixedString& filename, RE::BSScrapArray<RE::hkbBehaviorGraph*>& hgraphs);
        static inline REL::Relocation<decltype(LoadBehaviorProject)> _LoadBehaviorProject;
    };

    class ValidClipHook
    {
        public:
        static bool Install()
        {
            __try {
                REL::Relocation<std::uintptr_t> target{ REL::RelocationID(63069, 63981), REL::Relocate(0x89, 0x8B) };
                auto &trampoline = SKSE::GetTrampoline();
                _IsValidClip = trampoline.write_call<5>(target.address(), IsValidClip);
                return true;
            }
            __except(EXCEPTION_EXECUTE_HANDLER) {
                return false;
            }
        }
        private:
        static bool IsValidClip(RE::AnimationFileManagerSingleton* a_manager, const RE::hkbContext& a_context, uint16_t animationBindingIndex, RE::BSSynchronizedClipGenerator* a_synchronizedClipGenerator,
        void* unk05, uint32_t *a_outUserData);
        static inline REL::Relocation<decltype(IsValidClip)> _IsValidClip;
        static inline std::unordered_set<uint16_t> portedAnimationIndices;
    };

    static void InstallHooks()
    {
        SKSE::AllocTrampoline(14 * 3);

        if (!ValidClipHook::Install()) {
            SKSE::log::error("ValidClipHook failed to install - trampoline write_call raised an exception");
        } else {
            SKSE::log::info("ValidClipHook installed");
        }

        if (!LoadBehaviorGraphHook::Install()) {
            SKSE::log::error("LoadBehaviorGraphHook failed to install - trampoline write_call raised an exception");
        } else {
            SKSE::log::info("LoadBehaviorGraphHook installed");
        }

        if (!LoadBehaviorProjectHook::Install()) {
            SKSE::log::error("LoadBehaviorProjectHook failed to install - trampoline write_call raised an exception");
        } else {
            SKSE::log::info("LoadBehaviorProjectHook installed");
        }
    }
}
