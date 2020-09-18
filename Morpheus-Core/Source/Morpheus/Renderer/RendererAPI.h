#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "RendererCore/Renderpass.h"
#include "RendererCore/Pipeline.h"
#include "RendererCore/VertexBuffer.h"

namespace Morpheus {

	class RendererAPI
	{
	public:
		enum class API { None = 0, Vulkan = 1 };
	
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Flush() = 0;

		virtual void Reset() = 0;
		
		virtual void SetViewport() = 0;
		virtual void SetClearColor(const Vector4& _Color) = 0;
		virtual void BindPipeline(const Ref<Pipeline>& _Pipeline) = 0;
		virtual void BeginRenderpass(const Ref<Renderpass>& _Renderpass) = 0;
		virtual void EndRenderpass(const Ref<Renderpass>& _Renderpass) = 0;

		virtual void DrawGeomerty(const Ref<VertexBuffer>& _VertexBuffer) = 0;

		inline static void SetAPI(const API& _API) { s_RenderAPI = _API; }
		inline static API GetAPI() { return s_RenderAPI; }

	private:
		static API s_RenderAPI;

	public:
		static Scope<RendererAPI> Create();
	};

}