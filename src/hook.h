#pragma once
#include <unordered_set>
#include "RE/H/hkbBehaviorReferenceGenerator.h"
namespace APoseFix 
{
    class LoadBehaviorGraphHook
    {
        public:
        static void Install()
        {
            // Down	p	sub_140B06D20+11B	call    sub_140B06D20
            //	p	sub_140BC9070+10B	call    sub_140BC9070 AE
            REL::Relocation<std::uintptr_t> target{REL::RelocationID(63030, 63948), REL::Relocate(0x11B, 0x10B)};
            SKSE::AllocTrampoline(14);
            auto &trampoline = SKSE::GetTrampoline();
            _Load = trampoline.write_call<5>(target.address(), Load);
        }
        private:
        static RE::hkbBehaviorGraph* Load(const char *a_folderRoot, const char *a_behaviorFilePath, uint64_t a_assetLoader, void* a_unk04);
        static inline REL::Relocation<decltype(Load)> _Load;
    };


    // Since we don't know the project folder paths, we hook project load to map them
    // Up	p	BShkbAnimationGraph__sub_140AEDD10+108	call    sub_140AFFFE0
    //	p	sub_140BB0800:loc_140BB0911	call    sub_140BC1B10
    class LoadBehaviorProjectHook
    {
        public:
        static void Install()
        {
            REL::Relocation<std::uintptr_t> target{ REL::RelocationID(62640, 63585), REL::Relocate(0x108, 0x111) };
            SKSE::AllocTrampoline(14);
            auto &trampoline = SKSE::GetTrampoline();
            _LoadBehaviorProject = trampoline.write_call<5>(target.address(), LoadBehaviorProject);
        }
        private:
        static RE::hkbBehaviorGraph* LoadBehaviorProject(const char* folder, const char* name, RE::hkbCharacter& hchar, uint64_t project_data_handle, RE::BSFixedString& filename, RE::BSScrapArray<RE::hkbBehaviorGraph*>& hgraphs);
        static inline REL::Relocation<decltype(LoadBehaviorProject)> _LoadBehaviorProject;
    };

    

    /*Up	p	AnimationFileManagerSingleton__Func2_140B0A150+B6	call    AnimationFileManagerSingleton__sub_140B0A5C0
    	p	AnimationFileManagerSingleton__Func1_140B09FB0+89	call    AnimationFileManagerSingleton__sub_140B0A5C0*/
        // Up	p	sub_140BCBB90:loc_140BCBC1B	call    sub_140BCC220
   class ValidClipHook
   {

    public:
    static void Install()
    {
        REL::Relocation<std::uintptr_t> target{ REL::RelocationID(63069, 63981), REL::Relocate(0x89, 0x8B) };
        SKSE::AllocTrampoline(14);
        auto &trampoline = SKSE::GetTrampoline();
        _IsValidClip = trampoline.write_call<5>(target.address(), IsValidClip);
    }
    private:
    /*
    (unsigned __int8)AnimationFileManagerSingleton::sub_140B0A5C0(
                          a1,
                          (void *)a2,
                          *(_WORD *)&a3[1]._pad_14[4],
                          (BSSynchronizedClipGenerator *)a4,
                          &a2a,
                          &a6) 
    */
    static bool IsValidClip(RE::AnimationFileManagerSingleton* a_manager, const RE::hkbContext& a_context, uint16_t animationBindingIndex, RE::BSSynchronizedClipGenerator* a_synchronizedClipGenerator,
    void* unk05, uint32_t *a_outUserData);
    static inline REL::Relocation<decltype(IsValidClip)> _IsValidClip;
    static inline std::unordered_set<uint16_t> portedAnimationIndices; // we don't need a lock because Converter already throttles
   };


    static void InstallHooks()
    {
        ValidClipHook::Install();
        LoadBehaviorGraphHook::Install();
        LoadBehaviorProjectHook::Install();
    }
}