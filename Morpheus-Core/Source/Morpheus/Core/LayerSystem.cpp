#include "Morppch.h"
#include "LayerSystem.h"

#include "Morpheus/Utilities/MorpheusLogger.h"

namespace Morpheus {

	Layer::Layer(const String& _Name)
		: m_DebugName(_Name)
	{
		MORP_CORE_WARN("[LAYER CONTAINER] Created: " + m_DebugName);
	}

	LayerContainer::~LayerContainer()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerContainer::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerContainer::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerContainer::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerContainer::PopOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

}