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

		inline static void SetCompile(const bool& _Value)
		{
			s_RendererAPI->SetCompile(_Value);
		}

		inline static void SetReady(const bool& _Value)
		{
			s_RendererAPI->SetReady(_Value);
		}

		inline static void SetClearColor(const Vector4& _ClearColor)
		{
			s_RendererAPI->SetClearColor(_ClearColor);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}