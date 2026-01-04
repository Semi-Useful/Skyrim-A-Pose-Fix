#pragma once

#include "RE/H/hkStringPtr.h"
#include "RE/H/hkbGenerator.h"

namespace RE
{
	class hkbBehaviorGraph;

	class hkbBehaviorReferenceGenerator : public hkbGenerator
	{
	public:
		inline static constexpr auto RTTI = RE::RTTI_hkbBehaviorReferenceGenerator;
		inline static constexpr auto VTABLE = RE::VTABLE_hkbBehaviorReferenceGenerator;

		// members
		hkStringPtr       behaviorName;  // 48
		hkbBehaviorGraph* behavior;      // 50
	};
	static_assert(sizeof(hkbBehaviorReferenceGenerator) == 0x58);

}