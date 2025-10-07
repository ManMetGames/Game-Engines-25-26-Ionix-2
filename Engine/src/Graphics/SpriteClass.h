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
		const string *texture;
		const char *file;
		SDL_Texture* img;
		SDL_Renderer* rend;
		SDL_Window* win;
		Sprite(const char *file, SDL_Renderer* renderer);

		void draw(const char* file);
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