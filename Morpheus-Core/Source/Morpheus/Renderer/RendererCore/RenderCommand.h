#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void Shutdown()
		{
			s_RendererAPI->Shutdown();
		}

		inline static void BindGraph(const Ref<RenderGraph>& _RenderGraph)
		{
			s_RendererAPI->BindGraph(_RenderGraph);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& _VertexArray)
		{
			s_RendererAPI->DrawIndexed(_VertexArray);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}