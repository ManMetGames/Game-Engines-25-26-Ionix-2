#include "Graphics/SpriteClass.h"

namespace IonixEngine
{
	void Sprite::loadTexture(const char *file, SDL_Renderer* renderer)
	{
		// renderer will likely be m_renderer (in window.h)
		texture = IMG_LoadTexture(renderer, file);
	}
}