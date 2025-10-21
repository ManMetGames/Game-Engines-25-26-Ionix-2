#include "Architecture/Application.h"
#include <string>
#include "Graphics/SpriteAnimation.h"

namespace IonixEngine
{

	SpriteAnimation::SpriteAnimation(const char* file) //COPY OF SPRITE CLASS
	{
		rend = Application::Get().GetWindow().GetSdlRenderer();
		fileName = file;

		spriteSheet = IMG_LoadTexture(Application::Get().GetWindow().GetSdlRenderer(), fileName);

		SDL_QueryTexture(spriteSheet, NULL, NULL, &size.x, &size.y); //used to give spritesheets x and y values
	}
	void SpriteAnimation::changeTexture(const char* file, SDL_Renderer* renderer)
	{
		// renderer will likely be m_renderer (in window.h)
		//img = IMG_LoadTexture(renderer, file);
	}

	void SpriteAnimation::draw(const int x, const int y, const int w, const int h) {

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
		SDL_RenderCopy(rend, spriteSheet, nullptr, &texr);

		//SDL_DestroyTexture
	}

	void SpriteAnimation::Animation(int row, int column) //cuts up sprite sheet into equal frames 
	{
		float individualSpriteX = size.x / column;
		float individualSpriteY = size.y / row;

		
	};


}
