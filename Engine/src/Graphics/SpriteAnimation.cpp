#include "Architecture/Application.h"
#include <string>
#include "Graphics/SpriteAnimation.h"

namespace IonixEngine
{

	SpriteAnimation::SpriteAnimation(const char* file, bool reverse) //COPY OF SPRITE CLASS
	{
		rend = Application::Get().GetWindow().GetSdlRenderer();
		fileName = file;

		spriteSheet = IMG_LoadTexture(Application::Get().GetWindow().GetSdlRenderer(), fileName);

		SDL_QueryTexture(spriteSheet, NULL, NULL, &size.x, &size.y); //used to give spritesheets x and y values

		frames = GetFrameCount();
		currentFrame = 0;
		reverseOnEnd = reverse;
		playReverse = false;
	}

	void SpriteAnimation::setRect(const int x, const int y, const int w, const int h) 
	{
		dest.x = x;
		dest.y = y;
		dest.w = w;
		dest.h = h;
	}

	void SpriteAnimation::drawFrame(int x, int y, int w, int h)

		//x and y are origin point of the rect/sprite to be dranw
		//for now y will be kept as 0 but this will change when we implement multiple rows
		
		//w and h are the size of each sprite frame in the spritesheet
	{
		setRect(0, 0, 64, 64); //setting the size of the rect to be drawn on

		//setting the area of the individual image we're drawing AKA the frame in the spritesheet we want to draw
		src.x = x + w * currentFrame;
		src.y = y;
		src.w = w;
		src.h = h;

		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, spriteSheet, &src, &dest);

		if (reverseOnEnd)
		{

			if (!playReverse)
			{
				currentFrame++;
			}
			else
			{
				currentFrame--;
			}

			if (currentFrame > frames)
			{
				playReverse = true;
			}
			else if (currentFrame < 0)
			{
				playReverse = false;
			}
		}	
		else
		{
			currentFrame++;
			if (currentFrame > frames)
			{
				currentFrame = 0;
			}
		}
	}

	int SpriteAnimation::GetFrameCount() //for now, assume the spritesheet is 1 row
	{
		int x = size.x / size.y;
		return x;		
	};
}
