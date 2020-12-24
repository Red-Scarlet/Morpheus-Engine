#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

namespace Morpheus { namespace Vulkan {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Flush() override;

		virtual void SetClearColor(const Vector4& _ClearColor) override;
		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) override;

		virtual RendererAPI::RendererStats GetRendererStats() override;
	};

}}