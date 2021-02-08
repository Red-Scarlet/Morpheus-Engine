#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"
#include "Morpheus/Renderer/RendererCore/RenderCommand.h"
#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/Camera.h"

#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

#include "Morpheus/Renderer/RendererComponents/RendererRenderBufferInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderPipelineInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderTaskInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererVertexArrayInfo.h"

#include "Morpheus/Renderer/RendererResources/VertexArray.h"
#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"


namespace Morpheus {

	class Renderer
	{
	public:	
		static void Init()
		{
			MORP_PROFILE_FUNCTION();

			ResourceCommand::RegisterComponent<RendererAllocationInfo>();
			ResourceCommand::RegisterComponent<RendererDeallocationInfo>();
			ResourceCommand::RegisterComponent<RendererResourceInfo>();

			ResourceCommand::RegisterComponent<RendererRenderBufferInfo>();
			ResourceCommand::RegisterComponent<RendererRenderPipelineInfo>();
			ResourceCommand::RegisterComponent<RendererRenderTaskInfo>();
			ResourceCommand::RegisterComponent<RendererVertexArrayInfo>();

			RenderCommand::Init();
			s_RenderData = new RenderData;

			float Vertices[3 * 4] =
			{
				0.25f, 0.25f, 0.0f,
				-0.25f, 0.25f, 0.0f, 
				-0.25f, -0.25f, 0.0f,
				0.25f, -0.25f, 0.0f
			};
			uint32 Indices[6] = { 0, 1, 2, 0, 2, 3 };

			s_RenderData->VAO = VertexArray::Create();
			s_RenderData->VBO = VertexBuffer::Create(Vertices, sizeof(Vertices));
			s_RenderData->IBO = IndexBuffer::Create(Indices, sizeof(Indices));

			s_RenderData->VAO->AddVertexBuffer(s_RenderData->VBO);
			s_RenderData->VAO->SetIndexBuffer(s_RenderData->IBO);

			SetupDeferred();
		}

		static void SetupDeferred()
		{
			// Concrete Values
			RenderBufferLayout RenderBuffer_Layout = {
				{ "BackBuffer",		RendererImageFormat::FORMAT_R8G8B8A8_UNORM,		RendererImageLayout::USAGE_PRESENT_ATTACHMENT,			RendererImageUsage::USAGE_COLOR_BIT },
				{ "ColorBuffer",	RendererImageFormat::FORMAT_R8G8B8A8_UNORM,		RendererImageLayout::USAGE_PRESENT_ATTACHMENT,			RendererImageUsage::USAGE_COLOR_BIT },
				{ "NormalBuffer",	RendererImageFormat::FORMAT_R8G8B8A8_UNORM,		RendererImageLayout::USAGE_PRESENT_ATTACHMENT,			RendererImageUsage::USAGE_COLOR_BIT },
				{ "DepthBuffer",	RendererImageFormat::FORMAT_D32_SFLOAT,			RendererImageLayout::USAGE_DEPTH_STENCIL_ATTACHMENT,	RendererImageUsage::USAGE_DEPTH_STENCIL_BIT }
			};

			RenderTaskLayout Deferred_Subpass = {
				{ "BackBuffer",		RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "ColorBuffer",	RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "NormalBuffer",	RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "DepthBuffer",	RenderBufferState::RENDER_DEPTH,	RendererImageLayout::USAGE_DEPTH_STENCIL_ATTACHMENT },
			};

			RenderTaskLayout Lighting_Subpass = {
				{ "ColorBuffer",	RenderBufferState::RENDER_IN,		RendererImageLayout::USAGE_SHADER_READ_ATTACHMENT },
				{ "NormalBuffer",	RenderBufferState::RENDER_IN,		RendererImageLayout::USAGE_SHADER_READ_ATTACHMENT },
				{ "DepthBuffer",	RenderBufferState::RENDER_IN,		RendererImageLayout::USAGE_DEPTH_STENCIL_READ_ATTACHMENT },

				{ "BackBuffer",		RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "ColorBuffer",	RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "NormalBuffer",	RenderBufferState::RENDER_OUT,		RendererImageLayout::USAGE_COLOR_ATTACHMENT },
				{ "DepthBuffer",	RenderBufferState::RENDER_DEPTH,	RendererImageLayout::USAGE_DEPTH_STENCIL_READ_ATTACHMENT }
			};

			RenderGraphLayout RenderGraph_Layout = {
				{ "Deferred",	RenderTaskType::RENDER_GEOMERTY },
				{ "Lighting",	RenderTaskType::RENDER_LIGHTING }
			};

			PipelineAttributeLayout DeferredPipeline_Layout = {
				{ "Position", PipelineAttributeType::Float3 },
				{ "UV", PipelineAttributeType::Float2 },
				{ "Color", PipelineAttributeType::Float3 },
				{ "Normal", PipelineAttributeType::Float3 },
				{ "Tangent", PipelineAttributeType::Float3 }
			};

			PipelineAttributeLayout LightingPipeline_Layout = {
			};

			PipelineDescriptorLayout DescriptorLayout = {
				{ 0, PipelineDescriptorType::DESCRIPTOR_TYPE_UNIFORM_BUFFER, PipelineDescriptorStage::DESCRIPTOR_VERTEX },
				{ 1, PipelineDescriptorType::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, PipelineDescriptorStage::DESCRIPTOR_FRAGMENT },
				{ 2, PipelineDescriptorType::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, PipelineDescriptorStage::DESCRIPTOR_FRAGMENT },
				{ 3, PipelineDescriptorType::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, PipelineDescriptorStage::DESCRIPTOR_FRAGMENT },
				{ 4, PipelineDescriptorType::DESCRIPTOR_TYPE_UNIFORM_BUFFER, PipelineDescriptorStage::DESCRIPTOR_FRAGMENT }
			};

			RenderPipelineStruct DeferredPipeline_Struct = {};
			DeferredPipeline_Struct.AttributeLayout = DeferredPipeline_Layout;
			DeferredPipeline_Struct.DescriptorLayout = DescriptorLayout;
			DeferredPipeline_Struct.VertexFile = "Assets/Shaders/Deferred.vert.spv";
			DeferredPipeline_Struct.FragmentFile = "Assets/Shaders/Deferred.frag.spv";

			RenderPipelineStruct LightingPipeline_Struct = {};
			LightingPipeline_Struct.AttributeLayout = LightingPipeline_Layout;
			LightingPipeline_Struct.DescriptorLayout = DescriptorLayout;
			LightingPipeline_Struct.VertexFile = "Assets/Shaders/Lighting.vert.spv";
			LightingPipeline_Struct.FragmentFile = "Assets/Shaders/Lighting.frag.spv";

			s_RenderData->RG = RenderGraph::Create(RenderGraph_Layout);
			s_RenderData->RG->SetRenderBufferLayout(RenderBuffer_Layout);
			s_RenderData->RG->AddRenderTaskLayout("Deferred", Deferred_Subpass);
			s_RenderData->RG->AddRenderTaskLayout("Lighting", Lighting_Subpass);
			s_RenderData->RG->AddRenderPipeline("Deferred", DeferredPipeline_Struct);
			s_RenderData->RG->AddRenderPipeline("Lighting", LightingPipeline_Struct);

		}

		static void Shutdown()
		{
			MORP_PROFILE_FUNCTION();

			delete s_RenderData;
			RenderCommand::Shutdown();
		}

		static void EndScene()
		{
			MORP_PROFILE_FUNCTION();

			//RenderCommand::DrawIndexed(s_RenderData->VAO);
		}
	
	private:
		struct RenderData { Ref<VertexArray> VAO; Ref<VertexBuffer> VBO; Ref<IndexBuffer> IBO; Ref<RenderGraph> RG; };
		static RenderData* s_RenderData;
	
	};

}