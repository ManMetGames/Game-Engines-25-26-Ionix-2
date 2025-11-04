#pragma once
#include <SDL.h>

namespace IonixEngine
{
	class Camera
	{
	public:
		float x;
		float y;
		float zoom;

		Camera(float startX = -1.0f, float startY = 0.0f, float startZoom = 1.0f);

		void handleInput(float deltaTime);
		void handleEvent(const Uint8* keystates);
		SDL_Rect apply(const SDL_Rect& worldRect) const;
	};
}