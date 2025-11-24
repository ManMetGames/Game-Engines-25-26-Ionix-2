#pragma once
#include "Architecture/ECS/Component.hpp"
#include <Architecture/ECS/Entity.hpp>

namespace IonixEngine
{
	class CollisionLayerComponent : public Component
	{
	public:

		uint32_t Layer;
		uint32_t Mask;

		CollisionLayerComponent(Entity* entity) : Component(entity, true, false, false), Layer(0), Mask(0)
		{

		}

		inline uint32_t LayerToBit(uint32_t layerIndex)
		{
			return 1 << layerIndex;
		}

		inline uint32_t CreateMask(const std::vector<uint32_t>& layerIndices)
		{
			uint32_t mask = 0;
			for (uint32_t index : layerIndices)
			{
				mask |= LayerToBit(index);
			}
			return mask;
		}

		void SetLayer(uint32_t layerIndex)
		{
			Layer = LayerToBit(layerIndex);
		}

		void SetMask(std::vector<uint32_t>layerIndices)
		{
			Mask = CreateMask(layerIndices);
		}

		void AddLayerToMask(uint32_t layerIndex)
		{
			Mask |= LayerToBit(layerIndex);
		}

		void RemoveLayerFromMask(uint32_t layerIndex)
		{
			Mask &= ~LayerToBit(layerIndex);
		}
	};
}