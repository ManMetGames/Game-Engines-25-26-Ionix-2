#include "AnimatedSpriteComponent.h"
#include <Graphics/QueueRenderer.h>
#include<SDL.h>

namespace IonixEngine {
	AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder, int w, int h) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = w;
		height = h;
		
		//calculating the total frame count
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		totalFrames =  size.x / size.y;

		currentFrame = 0;

		SpriteSize(50, 50);
		SpriteRotation(45);
		SetColours(1, 1, 1, .2);
	}

	void AnimatedSpriteComponent::Render(RenderData* data)
	{
		if (currentFrame > totalFrames - 1)
		{
			currentFrame = 0;
		}

		// src is the indivudal frame we're rendering
		src.x = size.y * currentFrame;
		src.y = 0;
		src.w = size.y;
		src.h = size.y;

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { src.x, src.y, src.w, src.h },
			zOrder,
			entity->rotation,
			entity
		});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED
		//SDL_Delay(60);

		currentFrame++;
	}

	void AnimatedSpriteComponent::SpriteSize(int x, int y)

	{
		width = x;
		height = y;
	}

	void AnimatedSpriteComponent::SpriteRotation(float angleInDegrees) //code uses radians but player will use degrees so it's more user friendly
	{
		float radianDegree = angleInDegrees * (M_PI / 180); //converts degrees entered into radian
		entity->rotation = radianDegree;
	}

	void AnimatedSpriteComponent::SetColours(int red, int green, int blue, int alpha)
	{
		//Uint32* passThis = (Uint32*)image->pixels;

		image = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32,
			0x00FF0000,
			0x0000FF00,
			0x000000FF,
			0xFF000000);




		red = RED;
		green = GREEN;
		blue = BLUE;
		alpha = ALPHA;

		SDL_SetSurfaceColorMod(image, red, green, blue);

		//SDL_GetRGBA(passThis[0], image->format, &RED, &GREEN, &BLUE, &ALPHA);
	}

}

/*
TODO
- UNDO COMMENTED FUNCTION IN UI
- multiple rows
- overall make more versatile/usable

*/
