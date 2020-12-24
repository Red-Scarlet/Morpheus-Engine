#pragma once

#include "Common.h"
#include "Morpheus/Utilities/DeltaTime.h"
#include "Morpheus/Events/Event.h"

namespace Morpheus {

	class Layer 
	{
	public:
		Layer(const String& Name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() { MORP_PROFILE_FUNCTION(); }
		virtual void OnDetach() { MORP_PROFILE_FUNCTION(); }
		virtual void OnUpdate(const DeltaTime& _Delta) { MORP_PROFILE_FUNCTION(); }
		virtual void OnRender() { MORP_PROFILE_FUNCTION(); }
		virtual void OnEvent(Event& event) { MORP_PROFILE_FUNCTION(); }
		virtual void OnImGuiRender() { MORP_PROFILE_FUNCTION(); }
		inline const String& GetName() const { return m_DebugName; }

	protected:
		String m_DebugName;

	};

	class LayerContainer
	{
	public:
		LayerContainer() = default;
		~LayerContainer();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		Vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		Vector<Layer*>::iterator end() { return m_Layers.end(); }
		Vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		Vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

	private:
		Vector<Layer*> m_Layers;
		uint32 m_LayerInsertIndex = 0;
	};

}