#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "Morpheus/Renderer/RendererResources/VertexArray.h"
#include "Morpheus/Renderer/RendererPipeline/RenderGraph.h"

namespace Morpheus {

	// TODO Add Full specs later
	struct RendererStats
	{
	public:
		char Renderer[24];
		char Processor[24];
		char GraphicsProcessor[24];
	};

	class RendererAPI
	{
	public:
		enum class API { None = 0, Vulkan = 1 };

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void BindGraph(const Ref<RenderGraph>& _RenderGraph) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) = 0;

		inline static void SetAPI(const API& _API) { s_RenderAPI = _API; }
		inline static API GetAPI() { return s_RenderAPI; }

	private:
		static API s_RenderAPI;

	public:
		static Scope<RendererAPI> Create();
	};

}