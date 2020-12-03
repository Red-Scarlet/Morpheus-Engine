#include "Morppch.h"
#include "VulkanExecutionStack.h"

namespace Morpheus {

	void VulkanExecutionStack::Reset()
	{
		m_Commands.clear();
	}

	void VulkanExecutionStack::AppendCommand(const Ref<VulkanCommand>& _Command)
	{
		m_Commands.push_back(_Command);
	}

	const VkCommandBuffer& VulkanExecutionStack::Compile()
	{
		for (auto& Command : m_Commands)
			Command->Execute();
		return m_Commands[m_Commands.size() - 1]->Retrieve();
	}

	Ref<VulkanExecutionStack> VulkanExecutionStack::Make()
	{
		return CreateRef<VulkanExecutionStack>();
	}

}