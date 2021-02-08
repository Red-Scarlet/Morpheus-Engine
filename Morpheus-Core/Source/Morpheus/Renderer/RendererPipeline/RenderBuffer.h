#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererTypes.h"

namespace Morpheus {

	struct RenderBufferElement
	{
	public:
		String Name;
		RendererImageFormat Format;
		RendererImageLayout Layout;
		RendererImageUsage Usage;
		uint32 AttachmentIndex = 0;

		RenderBufferElement() {}
		RenderBufferElement(const String& _Name, const RendererImageFormat& _Format, const RendererImageLayout& _Layout, const RendererImageUsage& _Usage)
			: Name(_Name), Format(_Format), Layout(_Layout), Usage(_Usage)
		{
		}
	};
	
	class RenderBufferLayout
	{
	public:
		RenderBufferLayout() = default;
		RenderBufferLayout(const std::initializer_list<RenderBufferElement>& element)
			: m_Elements(element)
		{
			uint32 Index = 0;
			for (auto& element : m_Elements) {
				element.AttachmentIndex = Index;
				Index++;
			}
		}

		Vector<RenderBufferElement>::iterator begin() { return m_Elements.begin(); }
		Vector<RenderBufferElement>::iterator end() { return m_Elements.end(); }
		Vector<RenderBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<RenderBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		Vector<RenderBufferElement> m_Elements;
	};


}