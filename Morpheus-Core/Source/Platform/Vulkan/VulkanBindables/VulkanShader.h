#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"
#include "Morpheus/Renderer/RendererBindables/Shader.h"

#include "VulkanBindable.h"

namespace Morpheus {

	// HARD CODED DIRECTORIES
	struct VulkanVertexAttributes
	{
	public:
		vk::PipelineVertexInputStateCreateInfo InputState;
		vk::VertexInputBindingDescription InputBinding;
		Vector<vk::VertexInputAttributeDescription> InputAttributes;
	};

	class VulkanShader : public VulkanBindable, public Shader
	{
	public:
		VulkanShader(const VertexAttributeLayout& _Layout, const ShaderInfo& _ShaderPath);
		virtual ~VulkanShader();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		virtual void Bind() override;
		virtual void SetLayout(const VertexAttributeLayout& _Layout) override { m_Layout = _Layout; }
		virtual const VertexAttributeLayout& GetLayout() const { return m_Layout; };

		void CompileUniform(const uint32& _ID);

	private:
		Vector<float8> ReadFile(const String& _Filepath);	// Make in engine utils

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanDescriptorPool> m_DescriptorPool;
		Ref<VulkanPipeline> m_Pipeline;

		VertexAttributeLayout m_Layout;
		ShaderInfo m_ShaderPath;

		VulkanVertexAttributes m_Attributes;
		VulkanPipelineInput m_ShaderModules;
		uint32 m_DescriptorCount = 0;

	public:
		static Ref<VulkanShader> Make(const VertexAttributeLayout& _Layout, const ShaderInfo& _ShaderPath);
	};

}