#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
	class Sprite
	{
	public:
		const char *texture;
		SDL_Texture* img;
		SDL_Renderer* rend;
		SDL_Window* win;
		Sprite(const char* file, SDL_Renderer* renderer);

		void draw();
		void changeSize();
		void move();
		void changeTexture(const char* file, SDL_Renderer* renderer);
		void clear();
		void del();
	private:

		float width;
		float height;

		int xPosition;
		int yPosition;

		// order variable?

	};
}