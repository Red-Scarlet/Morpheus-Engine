#pragma once

#include "Morpheus/Core/Common.h"
#include "RenderCommand.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererCore/Renderpass.h"
#include "RendererCore/Pipeline.h"
#include "RendererCore/VertexBuffer.h"

namespace Morpheus {

	class Renderer
	{
	public:
		static void Init()
		{
			RenderCommand::Init();

			s_Renderdata.Renderpass = Renderpass::Create();
			s_Renderdata.Pipeline = Pipeline::Create(s_Renderdata.Renderpass);

			const Vector<Vertex> Vertices = {
				{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f}},
				{{-0.5f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}},
				{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
				{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 0.0f}},
				{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
				{{0.0f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}}
			};
			s_Renderdata.VertexBuffer = VertexBuffer::Create(Vertices);
		}

		static void Shutdown()
		{
			RenderCommand::Shutdown();
		}

		static void BeginScene()
		{
			RenderCommand::Begin();

			RenderCommand::SetViewport();
			RenderCommand::SetClearColor({ 0.7, 0.5, 0.3, 1.0 });

			RenderCommand::BeginRenderpass(s_Renderdata.Renderpass);

			RenderCommand::BindPipeline(s_Renderdata.Pipeline);

			RenderCommand::DrawGeomerty(s_Renderdata.VertexBuffer);

			RenderCommand::EndRenderpass(s_Renderdata.Renderpass);

			RenderCommand::End();	
		}

		static void EndScene()
		{
			RenderCommand::Flush();
		}

		static void DrawVertexBuffer()
		{
		}

	private:
		struct Renderdata {
			Ref<Renderpass> Renderpass; 
			Ref<Pipeline> Pipeline;
			Ref<VertexBuffer> VertexBuffer;
		};
		static Renderdata s_Renderdata;
	
	};

}