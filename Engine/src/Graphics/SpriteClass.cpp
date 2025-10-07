#include "Graphics/SpriteClass.h"

namespace IonixEngine
{

	Sprite::Sprite(const char *file, SDL_Renderer* renderer)
	{
		rend = Application::Get().GetWindow().GetSdlRenderer();

	}
	void Sprite::changeTexture(const char *file, SDL_Renderer* renderer)
	{
		// renderer will likely be m_renderer (in window.h)
		//texture = IMG_LoadTexture(renderer, file);
	}

	void Sprite::draw() {

		// code for rendering an image
		// to get working in OnUpdate:
		// copy all of this 
		// replace rend with Application::Get().GetWindow().GetSdlRenderer()
		// replace img with file path
		img = IMG_LoadTexture(rend, texture);
		SDL_Rect texr;
		texr.x = 0;
		texr.y = 0;
		texr.w = 800;
		texr.h = 600;



		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);


		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, img, nullptr, &texr);
		SDL_RenderPresent(rend);
	}
}