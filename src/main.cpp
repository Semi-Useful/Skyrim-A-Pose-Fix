#include "hook.h"
#include "converter.h"
void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		if (APoseFix::Converter::HasErrors())
		{
			auto report = APoseFix::Converter::GetErrorReport();
			RE::DebugMessageBox(report.c_str());
			SKSE::log::error("{}", report);
			APoseFix::Converter::ClearErrors();
		}
        break;
	case SKSE::MessagingInterface::kNewGame:
		if (APoseFix::Converter::HasErrors())
		{
			auto report = APoseFix::Converter::GetErrorReport();
			RE::DebugMessageBox(report.c_str());
			SKSE::log::error("{}", report);
			APoseFix::Converter::ClearErrors();
		}
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

    auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}
	APoseFix::InstallHooks();
	return true;
}
