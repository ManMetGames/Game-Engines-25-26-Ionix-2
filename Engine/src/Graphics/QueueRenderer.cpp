#include "Graphics/QueueRenderer.h"
#include <vector>

void QueueRenderer::AddToQueue(string spriteName)
{
	sprites->push(spriteName);
}

void QueueRenderer::Merger(int arr[], int left, int mid, int right)
{
	int n1 = mid - left + 1;
	int n2 = right - mid;

	int leftHand[n1];
	int rightHand[n2];

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
		if (leftHand[i] < rightHand[j])
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

void QueueRenderer::OrderQueueByZ(queue<int>& sprites)
{
	int temp[(sprites.size())]; //Creates temporary array from queue

	for (int i = 0; i < sprites.size(); i++)
	{
		temp[i] = sprites.front();
	}

	MergeCaller(temp, 0, sprites.size() - 1);
}

void QueueRenderer::MergeCaller(int temp[])
{
	int length = sprites.size(); //Returns queue length
	int left = 0;				 
	int right = sprites.size() - 1;
	int mid = left + (right - left) / 2;

	MergeCaller(temp, left, mid);
	MergeCaller(temp, mid + 1, right);
	Merger(temp, left, mid, right)
}

void QueueRenderer::ClearQueue(queue<string>& sprites)
{
	queue<string> emptyQueue;
	swap(sprites, emptyQueue);
}

void QueueRenderer::
