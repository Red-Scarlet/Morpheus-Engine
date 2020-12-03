#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "VulkanCommand.h"

namespace Morpheus {

	class VulkanExecutionStack
	{
	public:
		void Reset();
		void AppendCommand(const Ref<VulkanCommand>& _Command);
		const VkCommandBuffer& Compile();

	private:
		Vector<Ref<VulkanCommand>> m_Commands;

	public:
		static Ref<VulkanExecutionStack> Make();
	};

}