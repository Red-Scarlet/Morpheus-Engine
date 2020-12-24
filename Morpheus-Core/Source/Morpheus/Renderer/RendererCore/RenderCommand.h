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

		inline static void Flush()
		{
			s_RendererAPI->Flush();
		}

		inline static void SetClearColor(const Vector4& _ClearColor)
		{
			s_RendererAPI->SetClearColor(_ClearColor);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& _VertexArray)
		{
			s_RendererAPI->DrawIndexed(_VertexArray);
		}

		inline static RendererAPI::RendererStats GetRendererStats()
		{
			return s_RendererAPI->GetRendererStats();
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}