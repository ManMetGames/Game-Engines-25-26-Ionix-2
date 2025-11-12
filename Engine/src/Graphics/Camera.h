#pragma once
#include <SDL.h>

namespace IonixEngine
{
	class Camera
	{
	public:
		float x, xOffset;
		float y, yOffset;
		int h, w;
		float zoom;
		bool isFocused;
		int camIndex;

		Camera(float startX = 0.0f, float startY = 0.0f, float startZoom = 1.0f, int height = 800, int width = 600, bool isFocused = false);

		void Init();

		void handleInput(float deltaTime);
		void MoveCamera(float deltaX, float deltaY);
	};
}