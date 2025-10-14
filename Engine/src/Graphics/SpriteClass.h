#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;

namespace IonixEngine
{
	class Sprite
	{
	public:
		const char *fileName;
		SDL_Texture* img;
		SDL_Renderer* rend;
		SDL_Window* win;
		Sprite(const char *file);

		void draw(const int x, const int y, const int w, const int h);
		void changeSize();
		void move();
		void changeTexture(const char* file, SDL_Renderer* renderer);
		void clear();
		void del();

		//

		void Flip();
		void Sorting(); //layees

	private:

		int width;
		int height;

		int xPosition;
		int yPosition;


		// order variable?

	};
}