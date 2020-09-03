#pragma once

// --- Core -----------------------------------
#include "Morpheus/Core/Application.h"
#include "Morpheus/Utilities/MorpheusLogger.h"
// --------------------------------------------

// --- Entry Point ----------------------------
#define ENTRYPOINT
#ifdef ENTRYPOINT

#ifdef MORP_PLATFORM_WINDOWS

extern Morpheus::Application* CreateApplication();

int main(int argc, char** argv)
{
	Morpheus::MorpheusLogger::Init();
	MORP_CORE_SPECIAL("Initialized Log!");
	MORP_CORE_SPECIAL("Entrypoint Initialized!");
	MORP_CORE_INFO("Engine Version: Aquamarine - Early Development");

	auto app = CreateApplication();

	app->Run();
	app->Stop();

	Morpheus::MorpheusLogger::Shutdown();
	delete app;
}

#endif

#endif
// --------------------------------------------