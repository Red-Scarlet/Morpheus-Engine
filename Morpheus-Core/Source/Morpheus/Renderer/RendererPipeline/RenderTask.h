#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererTypes.h"

#include "Morpheus/Renderer/RendererPipeline/RenderBuffer.h"

namespace Morpheus {

	enum class RenderBufferState
	{
		RENDER_NONE, RENDER_IN, RENDER_OUT, RENDER_DEPTH
	};

	struct RenderTaskElement
	{
	public:
		String Name;
		RenderBufferState State;
		RendererImageLayout Attachment;

		RenderTaskElement() {}
		RenderTaskElement(const String& _Name, const RenderBufferState& _State, const RendererImageLayout& _Attachment)
			: Name(_Name), State(_State), Attachment(_Attachment)
		{
		}
	};

	class RenderTaskLayout
	{
	public:
		RenderTaskLayout() {}
		RenderTaskLayout(const std::initializer_list<RenderTaskElement>& element)
			: m_Elements(element)
		{
		}

		Vector<RenderTaskElement>::iterator begin() { return m_Elements.begin(); }
		Vector<RenderTaskElement>::iterator end() { return m_Elements.end(); }
		Vector<RenderTaskElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<RenderTaskElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		Vector<RenderTaskElement> m_Elements;
	};

}