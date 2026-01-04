#include "AnimationClipDataSingleton.h"

namespace RE
{
    AnimationClipDataSingleton* AnimationClipDataSingleton::GetSingleton()
	{
		return *REL::Relocation<AnimationClipDataSingleton**>(RELOCATION_ID(515414, 401553));
	}

	bool AnimationClipDataSingleton::GetClipInformation(const BSFixedString& project_name, const BSFixedString& clip_name, AnimationClipData& ClipInformation)
	{
		using func_t = decltype(&AnimationClipDataSingleton::GetClipInformation);
		REL::Relocation<func_t> func{ RELOCATION_ID(31799, 32573) };  
		return func(this, project_name, clip_name, ClipInformation);
	}

	bool AnimationClipDataSingleton::ClipData::GetEventTime(const BSFixedString& event_name, float& time)
	{
		using func_t = decltype(&AnimationClipDataSingleton::ClipData::GetEventTime);
		REL::Relocation<func_t> func{ RELOCATION_ID(31803, 0) };  
		return func(this, event_name, time);
	}
}