#pragma once
#include <SDL.h>
#include "Graphics/Camera.h"
#include "LayerSystem/Layers/LayerInput.h"

namespace IonixEngine
{
	class CameraColour
	{
	public:
		CameraColour(Camera* targetCamera);

		void Update();

		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		SDL_Color GetColor() const { return currentColor; }

	private:
		Camera* camera;
		SDL_Color currentColor;
	};
}