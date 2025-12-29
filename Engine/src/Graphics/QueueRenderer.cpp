#include "Graphics/QueueRenderer.h"
#include <vector>
#include "Architecture/Application.h"
#include "SDL_render.h"


namespace IonixEngine {

	void QueueRenderer::DrawLine(int x1, int y1, int x2, int y2, bool hitColor)
	{

		SDL_Renderer* renderer = Application::Get().GetWindow().GetSdlRenderer();
		// Set the color for drawing
		if (!hitColor)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		}
		else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);


		// Draw the line
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
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

	void QueueRenderer::Sort()
	{
		if (sprites.empty()) return;

		std::vector<RenderCall> temp;
		temp.reserve(sprites.size());

		while (!sprites.empty())
		{
			temp.push_back(sprites.front());
			sprites.pop();
		}

		MergeSort(temp, 0, temp.size() - 1);

		for (const auto& call : temp)
		{
			sprites.push(call);
		}
	}

	void QueueRenderer::MergeSort(std::vector<RenderCall>& temp, int left, int right)
	{
		if (left >= right) {
			return;
		}

		int mid = left + (right - left) / 2;

		MergeSort(temp, left, mid);
		MergeSort(temp, mid + 1, right);
		Merger(temp, left, mid, right);
	}

	void QueueRenderer::ClearQueue(queue<RenderCall>& sprites)
	{
		queue<RenderCall> emptyQueue;
		swap(sprites, emptyQueue);
	}

}