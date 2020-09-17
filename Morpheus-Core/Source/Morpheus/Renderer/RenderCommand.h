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

		inline static void SetViewport()
		{
			s_RendererAPI->SetViewport();
		}

		inline static void SetClearColor(const Vector4& _Color)
		{
			s_RendererAPI->SetClearColor(_Color);
		}

		inline static void SetRenderpass(const Ref<Renderpass>& _Renderpass)
		{
			s_RendererAPI->SetRenderpass(_Renderpass);
		}

		inline static void SetPipeline(const Ref<Pipeline>& _Pipeline)
		{
			s_RendererAPI->SetPipeline(_Pipeline);
		}

		inline static void SetFramebuffer(const Ref<Framebuffer>& _Framebuffer)
		{
			s_RendererAPI->SetFramebuffer(_Framebuffer);
		}

		inline static void DrawGeomerty()
		{
			s_RendererAPI->DrawGeomerty();
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}