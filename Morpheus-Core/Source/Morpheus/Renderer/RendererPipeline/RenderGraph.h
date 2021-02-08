#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererTypes.h"

#include "Morpheus/Renderer/RendererPipeline/RenderBuffer.h"
#include "Morpheus/Renderer/RendererPipeline/RenderTask.h"
#include "Morpheus/Renderer/RendererPipeline/RenderPipeline.h"

namespace Morpheus {

	enum class RenderTaskType
	{
		RENDER_DEPTH,
		RENDER_GEOMERTY,
		RENDER_LIGHTING
	};

	struct RenderGraphElement
	{
	public:
		String Name;
		RenderTaskType Type;

		RenderGraphElement() {}
		RenderGraphElement(const String& _Name, const RenderTaskType& _Type)
			: Name(_Name), Type(_Type)
		{
		}
	};

	class RenderGraphLayout
	{
	public:
		RenderGraphLayout() = default;
		RenderGraphLayout(const std::initializer_list<RenderGraphElement>& element)
			: m_Elements(element)
		{
		}

		const uint32& Size() const { return m_Elements.size(); }
		Vector<RenderGraphElement>::iterator begin() { return m_Elements.begin(); }
		Vector<RenderGraphElement>::iterator end() { return m_Elements.end(); }
		Vector<RenderGraphElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<RenderGraphElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		Vector<RenderGraphElement> m_Elements;
	};

	class RenderGraph
	{
	public:
		RenderGraph(const RenderGraphLayout& _Layout);
		virtual ~RenderGraph();
		const Resource& GetResource() { return m_Resource; }

		void SetRenderBufferLayout(const RenderBufferLayout& _Layout);
		void AddRenderTaskLayout(const String& _Name, const RenderTaskLayout& _Layout);
		void AddRenderPipeline(const String& _Name, const RenderPipelineStruct& _Struct);

	private:
		Resource m_Resource;

	public:
		static Ref<RenderGraph> Create(const RenderGraphLayout& _Layout);
	};

}