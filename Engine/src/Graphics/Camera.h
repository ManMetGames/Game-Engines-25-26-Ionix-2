#pragma once
#include <SDL.h>
#include <SDL_render.h>
#include "Architecture/Application.h"

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
		int renderLayer;
		Uint8 bg_r, bg_g, bg_b, bg_a; // background color
		SDL_Color currentColor;
		
		Camera(float startX = 0.0f, float startY = 0.0f, int renderLayer = 0);


		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void Init();
		SDL_Color GetColor() const { return currentColor; }
		void MoveCamera(float deltaX, float deltaY, bool moveCamDelta = true);
		void SwitchCamera();
		void SetZoom(SDL_Renderer* renderer, float zoom);
		void ClearBackground(SDL_Renderer* renderer);
		void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		//Render Texture Functions
		void InitRenderTexture(SDL_Renderer* renderer);   // sets texture as render target
		void RenderToTexture(SDL_Renderer* renderer);     // renders/draws texture and resets
		SDL_Texture* GetRenderTexture() const;            // retives texture
		void RenderToScreen(SDL_Renderer* renderer, float posX, float posY, float sizeX, float sizeY);
		void Rotate(float angle);
		void RotateEntity(Entity* entity, float angle);
	};
}
