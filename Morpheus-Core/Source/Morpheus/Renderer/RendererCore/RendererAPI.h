#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "Morpheus/Renderer/RendererBindables/VertexArray.h"

namespace Morpheus {


	class RendererAPI
	{
	public:
		enum class API { None = 0, Vulkan = 1 };
		struct RendererStats { String GPU = ""; uint32 VRAM = 0; uint32 SHARED_RAM = 0; uint32 USED_VRAM = 0; };

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Flush() = 0;

		virtual void SetClearColor(const Vector4& _ClearColor) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) = 0;

		inline static void SetAPI(const API& _API) { s_RenderAPI = _API; }
		inline static API GetAPI() { return s_RenderAPI; }

		virtual RendererStats GetRendererStats() = 0;

	private:
		static API s_RenderAPI;

	public:
		static Scope<RendererAPI> Create();
	};

}