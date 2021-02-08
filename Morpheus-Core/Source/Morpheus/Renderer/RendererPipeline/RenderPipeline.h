#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererTypes.h"

namespace Morpheus {


	static uint32 PipelineAttributeTypeSize(PipelineAttributeType _Type)
	{
		switch (_Type)
		{
			case PipelineAttributeType::Float:			return 4;
			case PipelineAttributeType::Float2:			return 4 * 2;
			case PipelineAttributeType::Float3:			return 4 * 3;
			case PipelineAttributeType::Float4:			return 4 * 4;
			case PipelineAttributeType::Mat3:			return 4 * 9;
			case PipelineAttributeType::Mat4:			return 4 * 16;
			case PipelineAttributeType::Int:			return 4;
			case PipelineAttributeType::Int2:			return 4 * 2;
			case PipelineAttributeType::Int3:			return 4 * 3;
			case PipelineAttributeType::Int4:			return 4 * 4;
			case PipelineAttributeType::Bool:			return 1;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
		return 0;
	}

	struct PipelineAttributeElement
	{
		String Name;
		PipelineAttributeType Type;
		uint32 Size;
		uint32 Offset;

		PipelineAttributeElement() {}
		PipelineAttributeElement(const String& _Name, const PipelineAttributeType& _Type)
			: Name(_Name), Type(_Type), Size(PipelineAttributeTypeSize(_Type)), Offset(0)
		{
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case PipelineAttributeType::Float:			return 1;
				case PipelineAttributeType::Float2:			return 2;
				case PipelineAttributeType::Float3:			return 3;
				case PipelineAttributeType::Float4:			return 4;
				case PipelineAttributeType::Mat3:			return 9;
				case PipelineAttributeType::Mat4:			return 16;
				case PipelineAttributeType::Int:			return 1;
				case PipelineAttributeType::Int2:			return 2;
				case PipelineAttributeType::Int3:			return 3;
				case PipelineAttributeType::Int4:			return 4;
				case PipelineAttributeType::Bool:			return 1;
			}
			MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class PipelineAttributeLayout
	{
	public:
		PipelineAttributeLayout() {}

		PipelineAttributeLayout(const std::initializer_list<PipelineAttributeElement>& element)
			: m_Elements(element)
		{
			CalculateOffsetAndStride();
		}

		inline uint32 GetStride() const { return m_Stride; }
		inline const Vector<PipelineAttributeElement>& GetElements() const { return m_Elements; }

		Vector<PipelineAttributeElement>::iterator begin() { return m_Elements.begin(); }
		Vector<PipelineAttributeElement>::iterator end() { return m_Elements.end(); }

		Vector<PipelineAttributeElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<PipelineAttributeElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32 offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		Vector<PipelineAttributeElement> m_Elements;
		uint32 m_Stride = 0;
	};

	struct PipelineDescriptorElement
	{
	public:
		uint32 Binding;
		PipelineDescriptorType Type;
		PipelineDescriptorStage Stage;

		PipelineDescriptorElement() {}
		PipelineDescriptorElement(const uint32& _Binding, const PipelineDescriptorType& _Type, const PipelineDescriptorStage& _Stage)
			: Binding(_Binding), Type(_Type), Stage(_Stage)
		{
		}
	};

	class PipelineDescriptorLayout
	{
	public:
		PipelineDescriptorLayout() {}
		PipelineDescriptorLayout(const std::initializer_list<PipelineDescriptorElement>& element)
			: m_Elements(element)
		{
		}
		Vector<PipelineDescriptorElement>::iterator begin() { return m_Elements.begin(); }
		Vector<PipelineDescriptorElement>::iterator end() { return m_Elements.end(); }
		Vector<PipelineDescriptorElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<PipelineDescriptorElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		Vector<PipelineDescriptorElement> m_Elements;
	};

	struct RenderPipelineStruct
	{
	public:		
		PipelineAttributeLayout AttributeLayout;
		PipelineDescriptorLayout DescriptorLayout;
		String VertexFile;
		String FragmentFile;
	};

}