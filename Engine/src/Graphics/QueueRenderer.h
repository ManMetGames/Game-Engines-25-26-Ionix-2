#pragma once
#include <iostream>
#include <queue>
#include <list>
#include <string>
#include <array>
#include "Graphics/SpriteComponent.h"

//#include <mutex>
using namespace std;

namespace IonixEngine {

	struct RenderCall { //render data
		SDL_Texture* texture;
		SDL_Rect dest;
		SDL_Rect src;
		int32_t z;
		double angle;  // Rotation in degrees
		Uint8 r = 255;  // Color tint (default white = no tint)
		Uint8 g = 255;
		Uint8 b = 255;
		Uint8 a = 255;
		float rotation;
		int renderLayer;
	};

	class QueueRenderer //Singleton
	{

	private:
		//member variables
		queue<RenderCall> sprites; //change string type later to sprite/texture
		//static pointer
		static QueueRenderer* queueRendPtr;

		//MAYBEEE????? use mutex

	public:

		QueueRenderer(const QueueRenderer& obj) = delete; //prevent copis
		QueueRenderer();
		void AddToQueue(RenderCall sprite);
		void OrderQueueByZ(queue<RenderCall>& sprites);
		void RenderFromQueue();
		void ClearQueue(queue<RenderCall>& sprites);
		void DrawLine(float x1, float y1, float x2, float y2, bool hitColor);

		static QueueRenderer& Get()
		{
			static QueueRenderer instance;
			return instance;
		}

		//sort algorithm stuff
		void Merger(vector<RenderCall>& arr, int left, int mid, int right);
		void MergeCaller(queue<RenderCall>& sprites, vector<RenderCall>& arr, int left, int right);
		void ArrToQueueConverter(vector<RenderCall> temp, queue<RenderCall>& sprites);

	};
}