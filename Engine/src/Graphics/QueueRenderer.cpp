#include "Graphics/QueueRenderer.h"

namespace IonixEngine {
	void QueueRenderer::AddToQueue(SpriteComponent sprite)
	{
		sprites->push(sprite.GetAlias());
	}

	void QueueRenderer::ClearQueue(queue<string>& sprites)
	{
		queue<string> emptyQueue;
		swap(sprites, emptyQueue);
	}
}