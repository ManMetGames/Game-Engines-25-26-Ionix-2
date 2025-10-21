#include "Graphics/QueueRenderer.h"


void QueueRenderer::AddToQueue(string spriteName)
{
	sprites->push(spriteName);
}

void QueueRenderer::OrderQueueByZ(queue<string> &sprites, int z)
{
	//Merge sort algorithm 
	list<string> leftHand;
	list<string> rightHand;

	int length = sprites.size();
	for (int i = 0; i < (sprites.size()); i++) //Add the first half of queue to the left-hand list of the merge 
	{
		if (sprites.size() % 2 != 0)
		{
			if ((sprites.size() / 2) + 1 < i)
			{
				std::string curElement = sprites.front();
				leftHand.push_front(curElement);
				sprites.pop(); //Removes from front
				continue;
			}
		}
		else //If not odd (so even)
		{
			if (sprites.size() / 2 < i)
			{
				std::string curElement = sprites.front();
				leftHand.push_front(curElement);
				sprites.pop(); //Removes from front
				continue;
			}
		}
		std::string curElement = sprites.front();
		rightHand.push_front(curElement);
		sprites.pop();
		continue; //Not strictly needed
	}
}

void QueueRenderer::ClearQueue(queue<string>& sprites)
{
	queue<string> emptyQueue;
	swap(sprites, emptyQueue);
}
