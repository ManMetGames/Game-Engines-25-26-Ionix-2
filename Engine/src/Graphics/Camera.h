#pragma once
#include <SDL.h>
#include <SDL_render.h>

namespace IonixEngine
{
	class Camera
	{
	public:
		float x;
		float y;
		int h, w;
		float zoom;

		Camera(float startX = 0.0f, float startY = 0.0f, float startZoom = 1.0f, int height = 800, int width = 600);

		void handleInput(float deltaTime);
		void handleEvent();

		//Render Texture Functions
		void InitRenderTexture(SDL_Renderer* renderer);   // sets texture as render target
		void RenderToTexture(SDL_Renderer* renderer);     // renders/draws texture and resets
		SDL_Texture* GetRenderTexture() const;            // retives texture

	private:
		// --- New members ---
		SDL_Texture* renderTexture = nullptr;  // holds texture obj
		int rtWidth = 0;                       // texture dimentions
		int rtHeight = 0;
	};
}