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
    // class LoadAnimationBindingHook
    // { this is only for behaviors
    //     public:
    //     static void Install()
    //     {
    //         REL::Relocation<std::uintptr_t> target{ RE::VTABLE___BSResourceAssetLoader[0] };
    //         _Load = target.write_vfunc(0x3, Load);
    //     }
    //     private:
    //     static RE::hkBaseObject* Load(uint64_t a_assetLoader, const char* a_folderRoot, const char* a_filePath,  const RE::hkClass& refClass);
    //     static inline REL::Relocation<decltype(Load)> _Load;
    // };

    class CombinePathsHook
    {
        public:
/*

Direction	Type	Address	Text
Up	p	sub_1404DC410+594	call    sub_140C37400  X
Up	p	sub_1404DC410+6C1	call    sub_140C37400 ? 
Up	p	sub_1404E06F0+6F3	call    sub_140C37400
Up	p	sub_1404E06F0+772	call    sub_140C37400
Up	p	sub_1404E06F0+954	call    sub_140C37400 
Up	p	sub_1404E42B0+175	call    sub_140C37400  X
Up	p	sub_1404E44F0+138	call    sub_140C37400 X
Up	p	sub_1404E6FD0+AB	call    sub_140C37400 X
Up	p	sub_14071BD40+75	call    sub_140C37400 X 
Up	p	sub_14071C3E0+7C	call    sub_140C37400 player responses 
Up	p	hkbBehaviorGraph__sub_140B00230+B7	call    sub_140C37400
Up	p	sub_140B00CE0+8A	call    sub_140C37400 X
Up	p	sub_140B00CE0+146	call    sub_140C37400 winner winner chicken dinner
Up	p	sub_140B023B0+AA	call    sub_140C37400 X 
Up	p	sub_140B023B0+D2	call    sub_140C37400 X
Up	p	sub_140B024E0+B8	call    sub_140C37400 X 
Up	p	sub_140B02620+1AB	call    sub_140C37400
Up	p	anonymous_namespace__BSResourceAssetLoader__Func3_140B02DE0+5B	call    sub_140C37400 X behaviors only
Up	p	sub_140B06670+3C	call    sub_140C37400 X 
Up	p	sub_140B06E90+40	call    sub_140C37400 X
Up	p	sub_140B079E0+55	call    sub_140C37400 X
Up	p	sub_140C2AD30+79	call    sub_140C37400 X
Down	o	.rdata:0000000141C70090	RUNTIME_FUNCTION <rva sub_140C37400, rva loc_140C3740F, \
Down	o	.rdata:0000000141C700A0	RUNTIME_FUNCTION <rva sub_140C37400, rva loc_140C3740F, \
Down	o	.pdata:000000014355CCBC	RUNTIME_FUNCTION <rva sub_140C37400, rva loc_140C3740F, \



Up	p	sub_140B00CE0+146	call    sub_140C37400 winner winner chicken dinner
use this to map relative filpaths to absolute?
*/
        static void Install()
        {
            REL::Relocation<std::uintptr_t> target{ REL::RelocationID(62927, 0), REL::Relocate(0x146, 0) };
            SKSE::AllocTrampoline(14);
            auto &trampoline = SKSE::GetTrampoline();
            _CombinePaths = trampoline.write_call<5>(target.address(), CombinePaths);
        }
        private:
        static bool CombinePaths(const char* a_path1, const char* a_path2, RE::BSFixedString* a_finalPath); 
        static inline REL::Relocation<decltype(CombinePaths)> _CombinePaths;
    };

    // Since we don't know the project folder paths, we hook project load to map them
    // Up	p	BShkbAnimationGraph__sub_140AEDD10+108	call    sub_140AFFFE0
    class LoadBehaviorProjectHook
    {
        public:
        static void Install()
        {
            REL::Relocation<std::uintptr_t> target{ REL::RelocationID(62640, 0), REL::Relocate(0x108, 0) };
            SKSE::AllocTrampoline(14);
            auto &trampoline = SKSE::GetTrampoline();
            _LoadBehaviorProject = trampoline.write_call<5>(target.address(), LoadBehaviorProject);
        }
        private:
        static RE::hkbBehaviorGraph* LoadBehaviorProject(const char* folder, const char* name, RE::hkbCharacter& hchar, uint64_t project_data_handle, RE::BSFixedString& filename, RE::BSScrapArray<RE::hkbBehaviorGraph*>& hgraphs);
        static inline REL::Relocation<decltype(LoadBehaviorProject)> _LoadBehaviorProject;
    };

    /*
    Direction	Type	Address	Text
Up	p	sub_14017A730+173	call    sub_140C37710
Up	p	sub_14035A890+16C	call    sub_140C37710
Up	p	sub_1404E42B0+19D	call    sub_140C37710
Up	p	sub_1404E44F0+160	call    sub_140C37710
Up	p	hkbBehaviorGraph__sub_140B00230+D8	call    sub_140C37710
	p	sub_140B00CE0+167	call    sub_140C37710
Down	p	anonymous_namespace__BSResourceAssetLoader__Func3_140B02DE0+80	call    sub_140C37710
Down	p	sub_140C38BA0+1D7	call    sub_140C37710
Down	o	.pdata:000000014355CCF8	RUNTIME_FUNCTION <rva sub_140C37710, rva algn_140C377FA, \
    */

    class ClipLoadHook
    {
        public:
        static void Install()
        {
            REL::Relocation<std::uintptr_t> target { RE::VTABLE_AnimationFileManagerSingleton[0] };
            _Queue = target.write_vfunc(0x1, Queue);
            _Load = target.write_vfunc(0x2, Load);
        }
        private:
        static int32_t Queue(RE::AnimationFileManagerSingleton* a_manager, RE::hkbContext& a_context, RE::hkbClipGenerator* a_clipGen, RE::BSSynchronizedClipGenerator* a_syncClipGen);
        static inline REL::Relocation<decltype(Queue)> _Queue;

        static bool Load(RE::AnimationFileManagerSingleton* a_manager, const RE::hkbContext& a_context, RE::hkbClipGenerator* a_clipGenerator, RE::BSSynchronizedClipGenerator* a_synchronizedClipGenerator);
        static inline REL::Relocation<decltype(Load)> _Load;
    };
    /*
Direction	Type	Address	Text
Up	p	hkbCharacter__sub_140B01670+41	call    sub_140B66930
Up	p	hkbCharacter__sub_140B016D0+52	call    sub_140B66930
Up	p	sub_140B06480+171	call    sub_140B66930
Up	p	hkbCharacter__sub_140B62650+54	call    sub_140B66930
Down	p	hkbCharacter__sub_140B685B0+3F	call    sub_140B66930
Down	o	.rdata:stru_141C567B4	IPtoStateMap <rva sub_140B66930, -1>
Down	o	.pdata:00000001435501C4	RUNTIME_FUNCTION <rva sub_140B66930, rva algn_140B66A39, \
    */

    /*Up	p	AnimationFileManagerSingleton__Func2_140B0A150+B6	call    AnimationFileManagerSingleton__sub_140B0A5C0
    	p	AnimationFileManagerSingleton__Func1_140B09FB0+89	call    AnimationFileManagerSingleton__sub_140B0A5C0*/
   class ValidClipHook
   {

    public:
    static void Install()
    {
        REL::Relocation<std::uintptr_t> target{ REL::RelocationID(63069, 0), REL::Relocate(0x89, 0) };
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
    static std::unordered_set<uint16_t> portedAnimationIndices; // we don't need a lock because Converter already throttles
   };

   class ClipGeneratorActivateHook
   {
        public:
        static void Install()
        {
            REL::Relocation<uintptr_t> hkbClipGeneratorVtbl{ RE::VTABLE_hkbClipGenerator[0] };
            _Activate = hkbClipGeneratorVtbl.write_vfunc(0x4, Activate);
        }
        private:
        static void Activate(RE::hkbClipGenerator* a_this, const RE::hkbContext& a_context);
        static inline REL::Relocation<decltype(Activate)> _Activate;
   };
    static void InstallHooks()
    {
        // LoadAnimationBindingHook::Install();
        // CombinePathsHook::Install();
        ValidClipHook::Install();
        // ClipLoadHook::Install();
        LoadBehaviorGraphHook::Install();
        LoadBehaviorProjectHook::Install();
    }
}