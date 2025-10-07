#include "Graphics/SpriteClass.h"
#include "Architecture/Application.h"
#include <string>

namespace IonixEngine
{

	Sprite::Sprite(const char *file, SDL_Renderer* renderer)
	{
		rend = Application::Get().GetWindow().GetSdlRenderer();

	}
	void Sprite::changeTexture(const char *file, SDL_Renderer* renderer)
	{
		// renderer will likely be m_renderer (in window.h)
		//img = IMG_LoadTexture(renderer, file);
	}

	void Sprite::draw(const char* fileName) {

		// code for rendering an image
		// to get working in OnUpdate:
		// copy all of this 
		// replace rend with Application::Get().GetWindow().GetSdlRenderer()
		// replace img with file path
		img = IMG_LoadTexture(Application::Get().GetWindow().GetSdlRenderer(), fileName);
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