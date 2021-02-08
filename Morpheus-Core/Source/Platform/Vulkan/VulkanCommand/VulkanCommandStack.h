#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommands.h"

namespace Morpheus { namespace Vulkan {

	class VulkanCommandStack
	{
	public:
		VulkanCommandStack();

		template<typename T>
		Ref<T> Push(const Ref<IVulkanCommand>& _Command)
		{
			m_Compiled = false;
			m_Commands.push_back(_Command);
			return std::dynamic_pointer_cast<T>(m_Commands.back());
		}
		
		template<typename T>
		Ref<T> Get()
		{
			for (Ref<IVulkanCommand> command : m_Commands) {
				const float8* templateName = typeid(T).name();
				const float8* commandName = typeid(command).name();
				if(templateName == commandName)
					return std::dynamic_pointer_cast<T>(command);
			}
			return nullptr;
		}

		void Pop(const Ref<IVulkanCommand>& _Command);
		void Execute(const Ref<VulkanCommandBuffer>& _Buffer);
		bool IsCompiled() { return m_Compiled; }

	private:
		Vector<Ref<IVulkanCommand>> m_Commands;
		bool m_Compiled = false;

	public:
		static Ref<VulkanCommandStack> Create();
	};

}}