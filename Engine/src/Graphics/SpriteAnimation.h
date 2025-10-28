#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;

namespace IonixEngine
{
	class SpriteAnimation
	{
	public:
		SDL_Texture* spriteSheet;
		SDL_Renderer* rend;
		SDL_Window* win;
		SDL_Point size;
		SDL_Rect dest;
		SDL_Rect src;

		const char* fileName;
		int frames;
		int currentFrame;
		bool reverseOnEnd; //if true, the animation will go back and forth
		bool playReverse;
		

		SpriteAnimation(const char* file, bool reverse);

		void drawFrame(int x, int y, int w, int h);
		void setRect(const int x, const int y, const int w, const int h);
		int GetFrameCount();
	};
}