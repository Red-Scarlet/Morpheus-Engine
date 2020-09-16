#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererAPI.h"
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

		inline static void Begin()
		{
			s_RendererAPI->Begin();
		}

		inline static void End()
		{
			s_RendererAPI->End();
		}

		inline static void Flush()
		{
			s_RendererAPI->Flush();
		}

		inline static void Reset()
		{
			s_RendererAPI->Reset();
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}