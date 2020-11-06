#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"
#include "Morpheus/Renderer/RendererBindables/Shader.h"
#include "Platform/Vulkan/VulkanBindable.h"

namespace Morpheus {

	// HARD CODED DIRECTORIES

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const String& _VertexPath, const String& _FragmentPath);
		virtual ~VulkanShader();
		void Destory();

		virtual void Bind() override;
		const uint32& GetID() { return m_Identifier.ID; }
		void SetID(const uint32& _ID) { m_Identifier.ID = _ID; }

		void AddToBindables(const VulkanBindableIdentifier& _Identifer);
		void CompileUniform(const uint32& _ID);

	private:
		Vector<float8> ReadFile(const String& _Filepath);
		void CreateShader(const String& _VertexPath, const String& _FragmentPath);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanDescriptorPool> m_DescriptorPool;
		Ref<VulkanPipeline> m_Pipeline;

		VulkanPipelineInput m_ShaderModules;

		VulkanBindableIdentifier m_Identifier;
		Vector<VulkanBindableIdentifier> m_Bindables;

		uint32 m_DescriptorCount = 0;

	public:
		static Ref<VulkanShader> VulkanCreate(const String& _VertexPath, const String& _FragmentPath);
	};

}