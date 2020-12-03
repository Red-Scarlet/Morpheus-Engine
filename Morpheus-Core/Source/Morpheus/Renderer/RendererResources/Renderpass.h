#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	
	enum class RenderpassLoadAttachment : uint8
	{
		ATTACHMENT_LOAD,
		ATTACHMENT_CLEAR,
		ATTACHMENT_DONTCARE
	};

	enum class RenderpassStoreAttachment : uint8
	{
		ATTACHMENT_STORE,
		ATTACHMENT_DONTCARE
	};

	enum class RenderpassTypes : uint8
	{
		ATTACHMENT_COLOR,
		ATTACHMENT_DEPTH
	};

	struct RenderpassElement
	{
		RenderpassTypes Type;
		RenderpassLoadAttachment LoadAttachment;
		RenderpassStoreAttachment StoreAttachment;

		RenderpassElement() {}
		RenderpassElement(RenderpassTypes _Types, RenderpassLoadAttachment _LoadAttachment, RenderpassStoreAttachment _StoreAttachment)
			: Type(_Types), LoadAttachment(_LoadAttachment), StoreAttachment(_StoreAttachment)
		{
		}

	};

	class RenderpassLayout
	{
	public:
		RenderpassLayout() {}

		RenderpassLayout(const std::initializer_list<RenderpassElement>& _Element)
			: m_Elements(_Element)
		{
		}

		inline const Vector<RenderpassElement>& GetElements() const { return m_Elements; }
		Vector<RenderpassElement>::iterator begin() { return m_Elements.begin(); }
		Vector<RenderpassElement>::iterator end() { return m_Elements.end(); }

		Vector<RenderpassElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<RenderpassElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		Vector<RenderpassElement> m_Elements;

	};

	class Renderpass
	{
	public:
		virtual ~Renderpass() = default;
		virtual const uint32& GetID() const = 0;

	public:
		static Ref<Renderpass> Create(const RenderpassLayout& _Layout);

	};

}