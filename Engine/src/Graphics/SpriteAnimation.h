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
		const char* fileName;
		SDL_Texture* spriteSheet;
		SDL_Renderer* rend;
		SDL_Window* win;
		int row;
		int column;
		SDL_Point size;

		SpriteAnimation(const char* file);

		void draw(const int x, const int y, const int w, const int h);
		void changeSize();
		void move();
		void changeTexture(const char* file, SDL_Renderer* renderer);
		void clear();
		void del();

		

		void Flip();
		void Sorting(); //layees
		void Animation(int spritesheetX, int spritesheetY);

	private:

		int width;
		int height;

		int xPosition;
		int yPosition;


		// order variable?

	};
}