#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/ResourceManager/ResourceUnit.h"

#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererVertexArrayInfo.h"

#include "Platform/Vulkan/VulkanComponents/VulkanCommandInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanExecutionInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanQueueInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanRendererInfo.h"


#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommands.h"


namespace Morpheus { namespace Vulkan {

	void VulkanRendererAPI::Init()
	{
		MORP_PROFILE_FUNCTION();

		m_Pool = VulkanCommandPool::Create();

		ResourceCommand::RegisterComponent<VulkanCommandInfo>();
		ResourceCommand::RegisterComponent<VulkanExecutionInfo>();
		ResourceCommand::RegisterComponent<VulkanMemoryInfo>();
		ResourceCommand::RegisterComponent<VulkanQueueInfo>();
		ResourceCommand::RegisterComponent<VulkanRendererInfo>();

		m_Allocator = VulkanAllocator::Create();
		ResourceCommand::RegisterSystem<VulkanAllocator>(m_Allocator);
		{
			ResourceSignature Signature;
			Signature.set(ResourceCommand::GetComponentType<RendererAllocationInfo>());
			ResourceCommand::SetSystemSignature<VulkanAllocator>(Signature);
		}
		m_Allocator->Init();

		m_Deallocator = VulkanDeallocator::Create();
		ResourceCommand::RegisterSystem<VulkanDeallocator>(m_Deallocator);
		{
			ResourceSignature Signature;
			Signature.set(ResourceCommand::GetComponentType<RendererDeallocationInfo>());
			ResourceCommand::SetSystemSignature<VulkanDeallocator>(Signature);
		}
		m_Deallocator->Init();

		m_Executor = VulkanExecutor::Create();
		ResourceCommand::RegisterSystem<VulkanExecutor>(m_Executor);
		{
			ResourceSignature Signature;
			Signature.set(ResourceCommand::GetComponentType<VulkanCommandInfo>());
			Signature.set(ResourceCommand::GetComponentType<VulkanExecutionInfo>());
			ResourceCommand::SetSystemSignature<VulkanExecutor>(Signature);
		}
		m_Executor->Init();

		m_Renderer = CreateRef<VulkanRenderer>();
		ResourceCommand::RegisterSystem<VulkanRenderer>(m_Renderer);
		{
			ResourceSignature Signature;
			Signature.set(ResourceCommand::GetComponentType<VulkanRendererInfo>());
			ResourceCommand::SetSystemSignature<VulkanRenderer>(Signature);
		}
		m_Renderer->Init();

		m_Updator = VulkanUpdator::Create();
		ResourceCommand::RegisterSystem<VulkanUpdator>(m_Updator);
		{
			ResourceSignature Signature;
			Signature.set(ResourceCommand::GetComponentType<RendererResourceInfo>());
			ResourceCommand::SetSystemSignature<VulkanUpdator>(Signature);
		}
		m_Updator->Init();

		ResourceUnitCache::GetAvailable()->AddSystem(m_Allocator);
		ResourceUnitCache::GetAvailable()->AddSystem(m_Deallocator);
		ResourceUnitCache::GetAvailable()->AddSystem(m_Executor);
		ResourceUnitCache::GetAvailable()->AddSystem(m_Renderer);
		ResourceUnitCache::GetAvailable()->AddSystem(m_Updator);

	}

	void VulkanRendererAPI::Shutdown()
	{
		MORP_PROFILE_FUNCTION();

		m_Updator->Stop();
		m_Renderer->Stop();
		m_Executor->Stop();
		m_Allocator->Stop();
		m_Deallocator->Stop();

		VulkanCommandPool::Destroy(m_Pool);
	}

	void VulkanRendererAPI::BindGraph(const Ref<RenderGraph>& _RenderGraph)
	{

	}

	// Draw VertexArray in currently bind or scoped renderpass
	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& _VertexArray)
	{
		//if (ResourceCommand::ContainsComponent<VulkanCommandInfo>(_VertexArray->GetResource()) == false) {
		//	VulkanCommandInfo CommandInfo = {};
		//	CommandInfo.Buffer = VulkanCommandBuffer::Create(m_Pool->GetCommandBuffer(true));
		//	CommandInfo.Stack = VulkanCommandStack::Create();
		//	CommandInfo.Stack->Push<CommandBeginBuffer>(CommandBeginBuffer::Create(true));
		//	CommandInfo.Stack->Push<CommandBindIndexBuffer>(CommandBindIndexBuffer::Create());
		//	CommandInfo.Stack->Push<CommandBindVertexBuffer>(CommandBindVertexBuffer::Create());
		//	CommandInfo.Stack->Push<CommandDrawIndexed>(CommandDrawIndexed::Create());
		//	CommandInfo.Stack->Push<CommandEndBuffer>(CommandEndBuffer::Create());
		//	ResourceCommand::AddComponent<VulkanCommandInfo>(_VertexArray->GetResource(), CommandInfo);
		//}
		//
		//if (ResourceCommand::ContainsComponent<RendererVertexArrayInfo>(_VertexArray->GetResource()) == true) {
		//	RendererVertexArrayInfo& VertexArrayInfo = ResourceCommand::GetComponent<RendererVertexArrayInfo>(_VertexArray->GetResource());
		//	if (VertexArrayInfo.Compiled == false) {
		//		VulkanCommandInfo& CommandInfo = ResourceCommand::GetComponent<VulkanCommandInfo>(_VertexArray->GetResource());
		//		if (ResourceCommand::ContainsComponent<VulkanMemoryInfo>(VertexArrayInfo.VertexBuffer) == true) {
		//			VulkanMemoryInfo& MemoryInfo = ResourceCommand::GetComponent<VulkanMemoryInfo>(VertexArrayInfo.VertexBuffer);
		//			if (MemoryInfo.Compiled)	// Todo: change buffer population to resource population.
		//				CommandInfo.Stack->Get<CommandBindVertexBuffer>()->PopulateData(
		//					std::dynamic_pointer_cast<VulkanVertexBuffer>(MemoryInfo.Instance)->GetBuffer());
		//		}
		//
		//		if (ResourceCommand::ContainsComponent<VulkanMemoryInfo>(VertexArrayInfo.IndexBuffer) == true) {
		//			VulkanMemoryInfo& MemoryInfo = ResourceCommand::GetComponent<VulkanMemoryInfo>(VertexArrayInfo.IndexBuffer);
		//			if (MemoryInfo.Compiled)// Todo: change buffer population to resource population.
		//				CommandInfo.Stack->Get<CommandBindIndexBuffer>()->PopulateData(
		//					std::dynamic_pointer_cast<VulkanIndexBuffer>(MemoryInfo.Instance)->GetBuffer());
		//		}
		//		VertexArrayInfo.Compiled = true;
		//	}
		//}
		//
		//if (ResourceCommand::ContainsComponent<VulkanCommandInfo>(_VertexArray->GetResource()) == true) {
		//	VulkanCommandInfo& CommandInfo = ResourceCommand::GetComponent<VulkanCommandInfo>(_VertexArray->GetResource());
		//	CommandInfo.Processed = CommandInfo.Stack->IsCompiled();
		//}
		//
		//if (ResourceCommand::ContainsComponent<VulkanExecutionInfo>(_VertexArray->GetResource()) == false) {
		//	auto const& CommandInfo = ResourceCommand::GetComponent<VulkanCommandInfo>(_VertexArray->GetResource());
		//	if (CommandInfo.Processed == false) {
		//		VulkanExecutionInfo ExecutionInfo = {};
		//		if (ResourceCommand::ContainsComponent<VulkanQueueInfo>(_VertexArray->GetResource()) == true)
		//			ResourceCommand::RemoveComponent<VulkanQueueInfo>(_VertexArray->GetResource());
		//		ResourceCommand::AddComponent<VulkanExecutionInfo>(_VertexArray->GetResource(), ExecutionInfo);
		//	}
		//}
		//
		//if (ResourceCommand::ContainsComponent<VulkanQueueInfo>(_VertexArray->GetResource()) == true) {
		//	VulkanQueueInfo& QueueInfo = ResourceCommand::GetComponent<VulkanQueueInfo>(_VertexArray->GetResource());
		//	QueueInfo.Ready = true;
		//}
	}

}}