#include "Morppch.h"
#include "VulkanExecutor.h"

#include "Platform/Vulkan/VulkanComponents/VulkanCommandInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanExecutionInfo.h"

namespace Morpheus { namespace Vulkan {

	VulkanExecutor::VulkanExecutor()
	{
	}

	void VulkanExecutor::Init()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanExecutor::Update()
	{
		MORP_PROFILE_FUNCTION();

		Vector<Resource> Executed;
		for (auto const& resource : m_Resources)
			if (Process(resource))
				Executed.emplace_back(resource);
		
		for (auto const& resource : Executed)
			ResourceCommand::RemoveComponent<VulkanExecutionInfo>(resource);
		Executed.clear();
	}

	void VulkanExecutor::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}
	
	bool VulkanExecutor::Process(const Resource& _Resource)
	{
		MORP_PROFILE_FUNCTION();

		auto const& ExecutionInfo = ResourceCommand::GetComponent<VulkanExecutionInfo>(_Resource);
		VulkanCommandInfo& CommandInfo = ResourceCommand::GetComponent<VulkanCommandInfo>(_Resource);
		CommandInfo.Processed = true;
		CommandInfo.Stack->Execute(CommandInfo.Buffer);
		VULKAN_CORE_INFO("[RESOURCE] Resource was Executed!");
		return CommandInfo.Stack->IsCompiled();
	}

	Ref<VulkanExecutor> VulkanExecutor::Create()
	{
		return CreateRef<VulkanExecutor>();
	}

}}