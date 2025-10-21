#pragma once
#include <iostream>
#include <queue>
#include <string>
#include "Graphics/SpriteComponent.h"

//#include <mutex>
using namespace std;

namespace IonixEngine {

	struct RenderCall { //render data
		SDL_Texture* texture;
		// SDL_Rect src;
		SDL_Rect dest;
		int32_t z;
	};

	class QueueRenderer //Singleton
	{

	private:
		//member variables
		queue<RenderCall> sprites; // queue of render data
		//static pointer

		//MAYBEEE????? use mutex

	public:
		QueueRenderer();
		QueueRenderer(const QueueRenderer& obj) = delete; //prevent copis
		void AddToQueue(RenderCall sprite);
		void RenderFromQueue();
		void ClearQueue(queue<RenderCall>& sprites);
		static QueueRenderer& Get()
		{
			static QueueRenderer instance;
			return instance;
		}

	};
}