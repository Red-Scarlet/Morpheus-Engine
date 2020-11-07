#include "Morppch.h"
#include "Renderer.h"

#include "Morpheus/Renderer/RendererSystems/Renderer2D.h"

namespace Morpheus {

	//Renderer::RenderData* Renderer::s_RenderData = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32 _Width, uint32 _Height)
	{
		// Do nothing
	}

}