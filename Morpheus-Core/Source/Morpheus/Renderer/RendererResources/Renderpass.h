#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	enum class RenderpassAttachmentLayout : uint8
	{
		ATTACHMENT_COLOR_OPTIMAL,
		ATTACHMENT_DEPTH_STENCIL_OPTIMAL,
		ATTACHMENT_DEPTH_STENCIL_READ,
		ATTACHMENT_SHADER_READ
	};

	enum class RenderpassFormat : uint8
	{
		ATTACHMENT_DEFAULT,
		ATTACHMENT_R8G8B8A8,
		ATTACHMENT_R32G32B32A32,
		ATTACHMENT_D32
	};
	
	enum class RenderpassLoad : uint8
	{
		ATTACHMENT_LOAD,
		ATTACHMENT_CLEAR,
		ATTACHMENT_DONTCARE
	};

	enum class RenderpassStore : uint8
	{
		ATTACHMENT_STORE,
		ATTACHMENT_DONTCARE
	};

	enum class RenderpassImage : uint8
	{
		ATTACHMENT_PRESENT,
		ATTACHMENT_UNDEFINED
	};

	struct RenderpassElement
	{
	public:
		RenderpassElement() = default;

		uint32 Attachment = 0;
		RenderpassAttachmentLayout Layout = RenderpassAttachmentLayout::ATTACHMENT_COLOR_OPTIMAL;

		RenderpassFormat Format = RenderpassFormat::ATTACHMENT_DEFAULT;
		RenderpassLoad Load = RenderpassLoad::ATTACHMENT_CLEAR;
		RenderpassStore Store = RenderpassStore::ATTACHMENT_STORE;
		RenderpassImage Initial = RenderpassImage::ATTACHMENT_UNDEFINED;
		RenderpassImage Final = RenderpassImage::ATTACHMENT_PRESENT;

		RenderpassElement* Next = nullptr;
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