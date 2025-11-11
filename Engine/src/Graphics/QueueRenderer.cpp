#include "Graphics/QueueRenderer.h"
#include <vector>

void QueueRenderer::AddToQueue(string spriteName)
{
	//sprites->push(spriteName); needs to be changed because this doesn't take strings anymore
}   //all of this is TEMPORARY because we're TESTING - it's in the name, that's why it says TEMP everywhere. We'll change this soon, thanks

void QueueRenderer::Merger(std::vector<int> arr, int left, int mid, int right)
{
	int n1 = mid - left + 1;
	int n2 = right - mid;

	std::vector<int> leftHand(n1);
	std::vector<int> rightHand(n2);

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
		if (leftHand[i] <= rightHand[j])
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

void QueueRenderer::OrderQueueByZ(queue<int>& sprites) //Called first
{
	std::vector<int> temp; //Creates temporary vector from queue

	for (int i = 0; i < sprites.size(); i++)
	{
		temp[i] = sprites.front();
	} //Adds all items from queue to this array to be sorted

	MergeCaller(temp, 0, sprites.size() - 1); //Perform merge sort
	ArrToQueueConverter(temp, sprites); //Convert vector to sorted queue again at the end!
}

void QueueRenderer::MergeCaller(vector<int> temp, int left, int right) //Takes the vector created from the queue and sorts it, called second
{
	if (left >= right)
	{
		return;
	}

	//int length = temp.size(); //Returns queue length
	//int left = 0;				 
	//int right = temp.size() - 1;
	int mid = left + (right - left) / 2;

	MergeCaller(temp, left, mid);
	MergeCaller(temp, mid + 1, right);
	Merger(temp, left, mid, right);
}

void QueueRenderer::ClearQueue(queue<int>& sprites)
{
	queue<int> emptyQueue;
	swap(sprites, emptyQueue);
}

void QueueRenderer::ArrToQueueConverter(std::vector<int> temp, queue<int>& sprites)
{
	ClearQueue(sprites);
	//std::queue<int> orderedQueue;
	for (int i = 0; i < temp.size(); i++)
	{
		sprites.push(temp[i]);
	}

	cout << sprites.size() << endl;
}
