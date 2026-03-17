#include "hook.h"
#include "converter.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
    switch (a_msg->type) {
    case SKSE::MessagingInterface::kPostLoadGame:
    case SKSE::MessagingInterface::kNewGame:
        if (APoseFix::Converter::HasErrors())
        {
            auto report = APoseFix::Converter::GetErrorReport();
            RE::DebugMessageBox(report.c_str());
            SKSE::log::error("{}", report);
            APoseFix::Converter::ClearErrors();
        }
        break;
    default:
        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    SKSE::GetLogger()->set_level(spdlog::level::info);
    SKSE::log::info("APoseFix starting load...");

    try {
        auto messaging = SKSE::GetMessagingInterface();
        if (!messaging->RegisterListener("SKSE", MessageHandler)) {
            SKSE::log::error("Failed to register message listener");
            return false;
        }
        SKSE::log::info("Registering message listener succeeded, installing hooks...");

        APoseFix::InstallHooks();

        SKSE::log::info("InstallHooks() completed");
    }
    catch (const std::exception& e) {
        SKSE::log::error("Fatal C++ exception during load: {}", e.what());
        return false;
    }
    catch (...) {
        SKSE::log::error("Fatal unknown exception during load");
        return false;
    }

    SKSE::log::info("APoseFix loaded successfully");
    return true;
}
