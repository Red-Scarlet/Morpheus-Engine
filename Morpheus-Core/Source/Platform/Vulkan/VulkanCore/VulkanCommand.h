#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCore/VulkanResource.h"
#include "Platform/Vulkan/VulkanCommand/VulkanInstructions.h"

namespace Morpheus { namespace Vulkan {

	class VulkanCommand : public ParentRef<VulkanCommand>, public VulkanResource
    {
    public:
        VulkanCommand() = default;

        void Accept(Ref<IResourceDispatcher>& _Dispatcher) override 
        { _Dispatcher->Dispatch(shared_from_this()); }

		template<typename T>
		Ref<T> MakeCommand(const VulkanCommandTypes& _Command, const Boolean& _IsSecondary = false)
		{
			MORP_PROFILE_FUNCTION();

			// Get a CommandBuffer from a dispatcher somewhere!
			// Likely The Executor Will provide one? or the pool.

			switch (_Command)
			{
				case VulkanCommandTypes::ResetBuffer:
					m_Commands.push_back(CommandResetBuffer::Create());
					break;
				case VulkanCommandTypes::BeginBuffer:
					m_Commands.push_back(CommandBeginBuffer::Create());
					break;
				case VulkanCommandTypes::EndBuffer:
					m_Commands.push_back(CommandEndBuffer::Create());
					break;
				case VulkanCommandTypes::CopyBufferToBuffer:
					m_Commands.push_back(CommandCopyBufferToBuffer::Create());
					break;
				case VulkanCommandTypes::CopyBufferToImage:
					m_Commands.push_back(CommandCopyBufferToImage::Create());
					break;
				case VulkanCommandTypes::CopyImageToImage:
					m_Commands.push_back(CommandCopyImageToImage::Create());
					break;
				case VulkanCommandTypes::BindBarrier:
					m_Commands.push_back(CommandBindBarrier::Create());
					break;
				case VulkanCommandTypes::SetViewport:
					m_Commands.push_back(CommandSetViewport::Create());
					break;
				case VulkanCommandTypes::SetScissor:
					m_Commands.push_back(CommandSetScissor::Create());
					break;
				case VulkanCommandTypes::SetClearColor:
					m_Commands.push_back(CommandSetClearColor::Create());
					break;
				case VulkanCommandTypes::BeginRenderpass:
					m_Commands.push_back(CommandBeginRenderpass::Create());
					break;
				case VulkanCommandTypes::EndRenderpass:
					m_Commands.push_back(CommandEndRenderpass::Create());
					break;
				case VulkanCommandTypes::BindPipeline:
					m_Commands.push_back(CommandBindPipeline::Create());
					break;
				case VulkanCommandTypes::DrawIndexed:
					m_Commands.push_back(CommandDrawIndexed::Create());
					break;
				case VulkanCommandTypes::BindVertexBuffer:
					m_Commands.push_back(CommandBindVertexBuffer::Create());
					break;
				case VulkanCommandTypes::BindIndexBuffer:
					m_Commands.push_back(CommandBindIndexBuffer::Create());
					break;
				case VulkanCommandTypes::BindDescriptorSet:
					m_Commands.push_back(CommandBindDescriptorSet::Create());
					break;
				case VulkanCommandTypes::ExecuteCommandBuffers:
					m_Commands.push_back(CommandExecuteCommandBuffers::Create());
					break;
			}

			return std::dynamic_pointer_cast<T>(m_Commands.at(m_Commands.size() - 1));
		}

		const Vector<Ref<VulkanInstruction>>& GetCommands()
		{ return m_Commands; }

		void Clear()
		{ m_Commands.clear(); }

    private:
        Vector<Ref<VulkanInstruction>> m_Commands;
		bool m_IsSecondary = false;

    public:
        static Ref<VulkanCommand> Create()
        { return CreateRef<VulkanCommand>(); }
    };

}}