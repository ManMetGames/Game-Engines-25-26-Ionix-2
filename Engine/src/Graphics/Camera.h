#pragma once
#include <SDL.h>
#include <SDL_render.h>
#include <cstdint>

namespace IonixEngine
{
	class Camera
	{
	public:
		float x, camDeltaX;
		float y, camDeltaY;
		int h, w;
		float zoom;
		bool isFocused;
		int camIndex;
		SDL_Texture* renderTexture = nullptr;  // holds texture obj
		int rtWidth = 0;                       // texture dimentions
		int rtHeight = 0;

		// Culling mask: bitflags for layers this camera will render.
		// Default = all bits set (renders everything).
		uint32_t cullingMask = 0xFFFFFFFFu;

		Camera(float startX = 0.0f, float startY = 0.0f, float startZoom = 1.0f, int height = 800, int width = 600, bool isFocused = false);

		void Init();

		// Culling mask API
		inline void SetCullingMask(uint32_t mask) { cullingMask = mask; }
		inline uint32_t GetCullingMask() const { return cullingMask; }

		void handleInput(float deltaTime);
		void MoveCamera(float deltaX, float deltaY, bool moveCamDelta = true);
		void SwitchCamera();

		//Render Texture Functions
		void InitRenderTexture(SDL_Renderer* renderer);   // sets texture as render target
		void RenderToTexture(SDL_Renderer* renderer);     // renders/draws texture and resets
		SDL_Texture* GetRenderTexture() const;            // retives texture
		void RenderToScreen(SDL_Renderer* renderer, float posX, float posY, float sizeX, float sizeY);
	};
}