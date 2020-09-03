#pragma once

namespace Morpheus {

	enum class RendererAPIs { None = 0, Vulkan };

	class RendererAPI
	{
	public:
		static RendererAPIs GetAPI() { return s_RenderAPI; }

	private:
		static RendererAPIs s_RenderAPI;

	};

}