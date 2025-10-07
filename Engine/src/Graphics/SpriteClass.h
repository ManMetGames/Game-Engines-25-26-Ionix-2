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
		SDL_Texture* texture;

		Sprite(const char* file, SDL_Renderer* renderer);

		void draw();
		void changeSize();
		void move();
		void loadTexture(const char* file, SDL_Renderer* renderer);
		void clear();
		void del();

		//

		void Flip();
		void Sorting(); //layees

	private:
		
		float baseWidth;
		float baseHeight;

		float width;
		float height;

		int xPosition;
		int yPosition;

		// order variable?

	};
}