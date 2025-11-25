#include "Graphics/QueueRenderer.h"
#include <vector>
#include <iostream>
#include "Architecture/Application.h"
#include "SDL_render.h"


namespace IonixEngine {

	QueueRenderer::QueueRenderer()
	{
		sprites = queue<RenderCall>();
	}

	void QueueRenderer::AddToQueue(RenderCall sprite)
	{
		sprites.push(sprite);
	}

	void QueueRenderer::RenderFromQueue() {
		if (sprites.size() >= 2)
		{
			OrderQueueByZ(sprites);
		}

		while (!sprites.empty()) {
			RenderCall call = sprites.front();
			SDL_RenderCopy(Application::Get().GetWindow().GetSdlRenderer(), call.texture, &call.src, &call.dest);
			sprites.pop();
		}
	}

	void QueueRenderer::Merger(std::vector<RenderCall>& temp, int left, int mid, int right)
	{
		int n1 = mid - left + 1;
		int n2 = right - mid;

		vector<RenderCall> leftHand(n1), rightHand(n2);

		for (int i = 0; i < n1; i++)
		{
			leftHand[i] = temp[left + i];
		}
		for (int j = 0; j < n2; j++)
		{
			rightHand[j] = temp[mid + 1 + j];
		}

		int i = 0, j = 0; int k = left;

		while (i < n1 && j < n2)
		{
			if (leftHand[i].z <= rightHand[j].z)
			{
				temp[k] = leftHand[i];
				i++;
			}
			else
			{
				temp[k] = rightHand[j];
				j++;
			}
			k++;
		}

		while (i < n1) //Add anything left from left-hand
		{
			temp[k] = leftHand[i];
			i++;
			k++;
		}

		while (j < n2) //Add anything left from right-hand
		{
			temp[k] = rightHand[j];
			j++;
			k++;
		}
	}

	void QueueRenderer::OrderQueueByZ(queue<RenderCall> sprites) //Called first
	{
		queue<RenderCall> tempQueue = sprites; //Creates temporary queue from sprites to avoid conflicts
		std::vector<RenderCall> tempVector; //Creates temporary vector from queue
		int originalQueueLength = sprites.size();

		for (int i = 0; i < originalQueueLength; i++) //Adds all items from queue to this vector to be sorted
		{
			tempVector.push_back(tempQueue.front());
			tempQueue.pop();
		}

		MergeCaller(tempVector, 0, tempVector.size() - 1); //Perform merge sort
		ArrToQueueConverter(tempVector, sprites); //Convert vector to sorted queue again at the end!

	}

	void QueueRenderer::MergeCaller(vector<RenderCall>& temp, int left, int right) //Takes the vector created from the queue and sorts it, called second
	{

		if (left < right)
		{
			int mid = left + (right - left) / 2;
			MergeCaller(temp, left, mid);
			MergeCaller(temp, mid + 1, right);
			Merger(temp, left, mid, right);
		}
	}

	void QueueRenderer::ClearQueue(queue<RenderCall>& sprites)
	{
		queue<RenderCall> emptyQueue;
		swap(sprites, emptyQueue);
	}

	void QueueRenderer::ArrToQueueConverter(std::vector<RenderCall> temp, queue<RenderCall>& sprites)
	{
		ClearQueue(sprites);
		for (int i = 0; i < temp.size(); i++)
		{
			sprites.emplace(temp[i]);
		}
	}
}
