#pragma once
#include <SDL.h>

namespace IonixEngine
{
	class Camera
	{
	public:
		float x;
		float y;
		int h, w;
		float zoom;

		Uint8 bg_r, bg_g, bg_b, bg_a;

		Camera(float startX = 0.0f, float startY = 0.0f, float startZoom = 1.0f,
			int height = 800, int width = 600,
			Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

		void handleInput(float deltaTime);
		void ClearBackground(SDL_Renderer* renderer);
		void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	};
}