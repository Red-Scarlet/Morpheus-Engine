#pragma once

// --- Core -----------------------------------
#include "Morpheus/Core/Application.h"
// --------------------------------------------

// --- Entry Point ----------------------------
#ifdef ENTRYPOINT

#ifdef MORP_PLATFORM_WINDOWS

extern Morpheus::Application* CreateApplication();

int main(int argc, char** argv)
{
	//Revealing::Log::Init();
	//RC_CORE_WARN("Initialized Log!");
	//RC_CORE_WARN("Entrypoint Initialized!");
	//RC_CORE_INFO("Engine Version: 1.0.0 Aquamarine - Early Development (OPEN SOURCE)");

	auto app = CreateApplication();

	app->Run();
	app->Stop();

	//Morpheus::Log::Shutdown();
	delete app;
}

#endif

#endif
// --------------------------------------------