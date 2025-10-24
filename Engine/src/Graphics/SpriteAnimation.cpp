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

		frames = GetFrameCount();
		currentFrame = 0;
	}

	void SpriteAnimation::setRect(const int x, const int y, const int w, const int h) 
	{
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
	}

	void SpriteAnimation::drawFrame(int x, int y, int w, int h)
	{
		setRect(0,0,64,64); //setting the size of the rect to be drawn on
		
		//setting the area of the image we're drawing
		src.x = x + w * currentFrame;
		src.y = y;
		src.w = w;
		src.h = h;

		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, spriteSheet, &src, &dest);

		currentFrame++;
		if (currentFrame > frames)
		{
			currentFrame = 0;
		}

	}

	int SpriteAnimation::GetFrameCount() //cuts up sprite sheet into equal frames 
	{
		int x = size.x / size.y;
		return x;		
	};


}
