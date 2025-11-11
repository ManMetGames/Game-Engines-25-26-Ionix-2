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
		bool isFocused;

		Camera(float startX = 0.0f, float startY = 0.0f, float startZoom = 1.0f, int height = 800, int width = 600, bool isFocused = false);

		void Init();

		void handleInput(float deltaTime);
	};
}