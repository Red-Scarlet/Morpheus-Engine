#pragma once

#include "Morpheus/Core/Common.h"
#include "RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererCore/Renderpass.h"
#include "RendererCore/Pipeline.h"
#include "RendererCore/Framebuffer.h"

namespace Morpheus {

	class Renderer
	{
	public:
		static void Init()
		{
			RenderCommand::Init();

			Ref<Renderpass> rp = Renderpass::Create();
			Ref<Pipeline> pipe = Pipeline::Create(rp);
			Ref<Framebuffer> fb = Framebuffer::Create(rp);

			RenderCommand::SetRenderpass(rp);
			RenderCommand::SetPipeline(pipe);
			RenderCommand::SetFramebuffer(fb);
			RenderCommand::SetClearColor({ 0.7, 0.5, 0.3, 1.0 });
			RenderCommand::SetViewport();
		}

		static void Shutdown()
		{
			RenderCommand::Shutdown();
		}

		static void BeginScene()
		{
			RenderCommand::Begin();
			RenderCommand::DrawGeomerty();
			RenderCommand::End();
		}

		static void EndScene()
		{
			RenderCommand::Flush();
		}

		static void DrawVertexBuffer()
		{
		}

	};

}