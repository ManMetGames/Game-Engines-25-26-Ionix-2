#include "Graphics/QueueRenderer.h"
#include "Architecture/Application.h"
#include "SDL_render.h"

namespace IonixEngine {

	QueueRenderer::QueueRenderer() {
		sprites = queue<RenderCall>();// queue of render data
	}

	void QueueRenderer::AddToQueue(RenderCall sprite)
	{
		sprites.push(sprite); // pushes render data onto sprite queue
	}

	void QueueRenderer::ClearQueue(queue<RenderCall>& sprites)
	{
		queue<RenderCall> emptyQueue;
		swap(sprites, emptyQueue);
	}

	void QueueRenderer::RenderFromQueue() {
		while (!sprites.empty()) {
			RenderCall call = sprites.front();
			SDL_RenderCopy(Application::Get().GetWindow().GetSdlRenderer(), call.texture, nullptr, &call.dest);
			sprites.pop();
		}
	}
}