#include "Graphics/SpriteClass.h"
#include "Architecture/Application.h"
#include <string>

namespace IonixEngine
{

	Sprite::Sprite(const char *file)
	{
		rend = Application::Get().GetWindow().GetSdlRenderer();
		fileName = file;
		img = IMG_LoadTexture(Application::Get().GetWindow().GetSdlRenderer(), fileName);

		SDL_QueryTexture(img, NULL, NULL, &size.x, &size.y);
		std::cout << "X of image: " + std::to_string(size.x)<< endl;
		std::cout << "Y of image: " + std::to_string(size.y)<< endl;

	}
	void Sprite::changeTexture(const char *file, SDL_Renderer* renderer)
	{
		// renderer will likely be m_renderer (in window.h)
		//img = IMG_LoadTexture(renderer, file);
	}

	void Sprite::draw(const int x, const int y, const int w, const int h, float angle) {

		// code for rendering an image
		// to get working in OnUpdate:
		// copy all of this 
		// replace rend with Application::Get().GetWindow().GetSdlRenderer()
		// replace img with file path
		SDL_Rect texr;
		texr.x = x;
		texr.y = y;
		texr.w = w;
		texr.h = h;


		

		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);

		SDL_RenderClear(rend);
		SDL_RenderCopyEx(rend, img, nullptr, &texr, angle, NULL, SDL_FLIP_NONE);

		//SDL_DestroyTexture
	}
}
