#pragma once

// --- Core -----------------------------------
#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Application.h"
#include "Morpheus/Core/LayerSystem.h"
#include "Morpheus/Core/Input.h"
// --------------------------------------------

// --- Event ----------------------------------
#include "Morpheus/Events/Event.h"
#include "Morpheus/Events/KeyEvent.h"
#include "Morpheus/Events/MouseEvent.h"
#include "Morpheus/Events/ApplicationEvent.h"
// --------------------------------------------

// --- Utils ----------------------------------
#include "Morpheus/Utilities/MorpheusLogger.h"
#include "Morpheus/Utilities/Instrumentor.h"
// --------------------------------------------

// --- Math -----------------------------------
#include "Morpheus/Mathematics/Mathematics.h"
// --------------------------------------------

// --- Renderer -------------------------------
#include "Morpheus/Renderer/Renderer.h"
#include "Morpheus/Renderer/Camera.h"
#include "Morpheus/Renderer/RendererCameras/PerspectiveCamera.h"
// --------------------------------------------

// --- Entry Point ----------------------------
#define ENTRYPOINT
#ifdef ENTRYPOINT

#ifdef MORP_PLATFORM_WINDOWS
extern Morpheus::Application* CreateApplication();

#ifdef MORP_DIST
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
#else
int main(int argc, char** argv)
#endif
{
	MORP_PROFILE_BEGIN_SESSION("Startup", "MorpheusProfile-Startup.json");
	auto app = CreateApplication();
	MORP_PROFILE_END_SESSION();

	MORP_PROFILE_BEGIN_SESSION("Runtime", "MorpheusProfile-Runtime.json");
	app->Run();
	MORP_PROFILE_END_SESSION();

	MORP_PROFILE_BEGIN_SESSION("Shutdown", "MorpheusProfile-Shutdown.json");
	app->Stop();
	MORP_PROFILE_END_SESSION();

	delete app;
	}
#endif
#endif
// --------------------------------------------