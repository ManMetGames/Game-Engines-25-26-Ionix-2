#include "Graphics/QueueRenderer.h"


void QueueRenderer::AddToQueue(string spriteName)
{

}

void QueueRenderer::ClearQueue(queue<string>& sprites)
{
	queue<string> emptyQueue;
	swap(sprites, emptyQueue);
}
