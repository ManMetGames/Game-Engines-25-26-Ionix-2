#pragma once
#include "Architecture/Macros.h"
#include "Layer.h"
#include <vector>

namespace IonixEngine
{
	class ENGINE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		Layer* GetLayerByID(int id);
		std::vector<Layer*> GetLayers() { return m_Layers; }

	private:
		std::vector<Layer*> m_Layers;
	};
}



