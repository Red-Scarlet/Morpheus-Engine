#include "Morppch.h"
#include "VulkanCommandStack.h"

namespace Morpheus { namespace Vulkan {

	VulkanCommandStack::VulkanCommandStack()
		: m_Compiled(false)
	{
	}

	void VulkanCommandStack::Pop(const Ref<IVulkanCommand>& _Command)
	{
		std::remove(std::begin(m_Commands), std::end(m_Commands), _Command);
		m_Compiled = false;
	}

	void VulkanCommandStack::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		uint8 CompilationFlag = 0x0000;
		for (Ref<IVulkanCommand> command : m_Commands) {
			CompilationFlag += command->Execute(_Buffer);
			if (CompilationFlag == 0) break;
		}
		if (CompilationFlag == m_Commands.size())
			m_Compiled = true;
		m_Compiled = false;
	}

	Ref<VulkanCommandStack> VulkanCommandStack::Create()
	{
		return CreateRef<VulkanCommandStack>();
	}

}}