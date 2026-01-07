#include "Graphics/QueueRenderer.h"
#include <vector>
#include "Architecture/Application.h"
#include "SDL_render.h"


namespace IonixEngine {

	void QueueRenderer::DrawLine(float x1, float y1, float x2, float y2, bool hitColor)
	{

		SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();
		// Set the color for drawing
		if (!hitColor)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		}
		else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);


		// Draw the line
		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	}

	void QueueRenderer::DrawGrid(float x, float y, float width, float height, float cellSize, SDL_Color color)
	{
		SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		// Draw vertical lines
		for (float i = 0; i <= width; i += cellSize)
		{
			float xPos = x + i;
			SDL_RenderDrawLineF(renderer, xPos, y, xPos, y + height);
		}

		// Draw horizontal lines
		for (float j = 0; j <= height; j += cellSize)
		{
			float yPos = y + j;
			SDL_RenderDrawLineF(renderer, x, yPos, x + width, yPos);
		}
	}
	
	QueueRenderer::QueueRenderer() {
		sprites = queue<RenderCall>();// queue of render data
	}

	void QueueRenderer::AddToQueue(RenderCall sprite)
	{
		sprites.push(sprite); //needs to be changed because this doesn't take strings anymore
	}   //all of this is TEMPORARY because we're TESTING - it's in the name, that's why it says TEMP everywhere. We'll change this soon, thanks

	void QueueRenderer::RenderFromQueue() {
		while (!sprites.empty()) {
			RenderCall call = sprites.front();
			if (call.renderLayer == Application::Get().currentCam->renderLayer) {
				// Apply color tint
				SDL_SetTextureColorMod(call.texture, call.r, call.g, call.b);
				// Apply alpha modulation
				SDL_SetTextureAlphaMod(call.texture, call.a);
				// Combine base entity rotation and any additional sprite rotation
				double finalAngle = call.angle + call.rotation;
				SDL_RenderCopyEx(Application::Get().GetWindow().GetSdlRenderer(), call.texture, &call.src, &call.dest, finalAngle, NULL, SDL_FLIP_NONE);
				// Reset color mod to white for next texture
				SDL_SetTextureColorMod(call.texture, 255, 255, 255);
				// Reset alpha mod to fully opaque for next texture
				SDL_SetTextureAlphaMod(call.texture, 255);
			}
			sprites.pop();
		}
	}

	void QueueRenderer::Merger(std::vector<RenderCall>& arr, int left, int mid, int right)
	{
		const int n1 = mid - left + 1;
		int n2 = right - mid;

		std::vector<RenderCall> leftHand(n1);
		std::vector<RenderCall> rightHand(n2);

		for (int i = 0; i < n1; i++)
		{
			leftHand[i] = arr[left + i];
		}
		for (int j = 0; j < n2; j++)
		{
			rightHand[j] = arr[mid + 1 + j];
		}
		int i = 0, j = 0, k = left;

		while (i < n1 && j < n2)
		{
			if (leftHand[i].z < rightHand[j].z)
			{
				arr[k] = leftHand[i];
				i++;
			}
			else
			{
				arr[k] = rightHand[j];
				j++;
			}
			k++;
		}

		while (i < n1) //Add anything left from left-hand
		{
			arr[k] = leftHand[i];
			i++;
			k++;
		}

		while (j < n2) //Add anything left from right-hand
		{
			arr[k] = rightHand[j];
			j++;
			k++;
		}
	}

	void QueueRenderer::OrderQueueByZ(queue<RenderCall>& sprites)
	{
		std::vector<RenderCall> temp(sprites.size());
		//int temp[(sprites.size())]; //Creates temporary array from queue

		for (int i = 0; i < sprites.size(); i++)
		{
			temp[i] = sprites.front();
		}

		MergeCaller(sprites, temp, 0, sprites.size() - 1);
		ArrToQueueConverter(temp, sprites);
	}

	void QueueRenderer::MergeCaller(queue<RenderCall>& sprites, std::vector<RenderCall>& temp, int left, int right)
	{

		if (left >= right) {
			return;
		}

		int length = sprites.size(); //Returns queue length
		/*left = 0;
		right = sprites.size() - 1;*/
		int mid = left + (right - left) / 2;

		MergeCaller(sprites, temp, left, mid);
		MergeCaller(sprites, temp, mid + 1, right);
		Merger(temp, left, mid, right);

		ArrToQueueConverter(temp, sprites); //Convert vector to queue at the end!
	}

	void QueueRenderer::ClearQueue(queue<RenderCall>& sprites)
	{
		queue<RenderCall> emptyQueue;
		swap(sprites, emptyQueue);
	}

	void QueueRenderer::ArrToQueueConverter(std::vector<RenderCall> temp, queue<RenderCall>& sprites)
	{
		ClearQueue(sprites);
		//std::queue<int> orderedQueue;
		for (int i = 0; i < temp.size(); i++)
		{
			sprites.push(temp[i]);
		}
	}
}