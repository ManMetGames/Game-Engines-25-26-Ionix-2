#pragma once
#include "Graphics/QueueRenderer.h"
#include <vector>
#include "Architecture/Application.h"
#include "SDL_render.h"


namespace IonixEngine {

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
			SDL_RenderCopy(Application::Get().GetWindow().GetSdlRenderer(), call.texture, nullptr, &call.dest);
			sprites.pop();
		}
	}

	void QueueRenderer::Merger(std::vector<RenderCall> arr, int left, int mid, int right)
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
	}

	void QueueRenderer::MergeCaller(queue<RenderCall>& sprites, std::vector<RenderCall> temp, int left, int right)
	{
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