#include "Morppch.h"
#include "RenderGraph.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

#include "Morpheus/Renderer/RendererComponents/RendererRenderBufferInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderPipelineInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderTaskInfo.h"

namespace Morpheus {

	RenderGraph::RenderGraph(const RenderGraphLayout& _Layout)
	{
		m_Resource = ResourceCommand::CreateResource();

		RendererRenderBufferInfo RenderBufferInfo = {};
		ResourceCommand::AddComponent<RendererRenderBufferInfo>(m_Resource, RenderBufferInfo);

		RendererRenderTaskInfo RenderTaskInfo = {};
		ResourceCommand::AddComponent<RendererRenderTaskInfo>(m_Resource, RenderTaskInfo);

		RendererRenderPipelineInfo RenderPipelineInfo = {};
		ResourceCommand::AddComponent<RendererRenderPipelineInfo>(m_Resource, RenderPipelineInfo);

		RendererAllocationInfo AllocInfo = {};
		AllocInfo.Type = RendererResourceTypes::RENDERER_RENDER_GRAPH;
		AllocInfo.Data = _Layout;
		ResourceCommand::AddComponent<RendererAllocationInfo>(m_Resource, AllocInfo);

		RendererResourceInfo ResourceInfo = {};
		ResourceInfo.Type = RendererResourceTypes::RENDERER_RENDER_GRAPH;
		ResourceCommand::AddComponent<RendererResourceInfo>(m_Resource, ResourceInfo);
	}

	RenderGraph::~RenderGraph()
	{
		ResourceCommand::RemoveComponent<RendererResourceInfo>(m_Resource);
		ResourceCommand::RemoveComponent<RendererRenderBufferInfo>(m_Resource);
		ResourceCommand::RemoveComponent<RendererRenderTaskInfo>(m_Resource);
		ResourceCommand::RemoveComponent<RendererRenderPipelineInfo>(m_Resource);

		RendererDeallocationInfo DeallocInfo = {};
		DeallocInfo.Type = RendererResourceTypes::RENDERER_RENDER_GRAPH;
		DeallocInfo.Flags = RendererResourceFlags::RENDERER_REMOVE_RESOURCE;
		ResourceCommand::AddComponent<RendererDeallocationInfo>(m_Resource, DeallocInfo);
	}

	// TODO: GENERALIZE COMPILATION (compiled) RESOURCE TAG (component)
	void RenderGraph::SetRenderBufferLayout(const RenderBufferLayout& _Layout)
	{
		RendererRenderBufferInfo& RenderBufferInfo = ResourceCommand::GetComponent<RendererRenderBufferInfo>(m_Resource);
		RenderBufferInfo.RenderBuffer = _Layout;

		RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
		ResourceInfo.Changed |= 0x01;
	}

	void RenderGraph::AddRenderTaskLayout(const String& _Name, const RenderTaskLayout& _Layout)
	{
		RendererRenderTaskInfo& RenderTaskInfo = ResourceCommand::GetComponent<RendererRenderTaskInfo>(m_Resource);
		RenderTaskInfo.RenderTasks.push_back({ _Name, _Layout });

		RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
		ResourceInfo.Changed |= 0x02;
	}

	void RenderGraph::AddRenderPipeline(const String& _Name, const RenderPipelineStruct& _Struct)
	{
		RendererRenderPipelineInfo& RenderPipelineInfo = ResourceCommand::GetComponent<RendererRenderPipelineInfo>(m_Resource);
		RenderPipelineInfo.RenderPipelines.push_back({ _Name, _Struct });

		RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
		ResourceInfo.Changed |= 0x04;
	}

	Ref<RenderGraph> RenderGraph::Create(const RenderGraphLayout& _Layout)
	{
		return CreateRef<RenderGraph>(_Layout);
	}


}