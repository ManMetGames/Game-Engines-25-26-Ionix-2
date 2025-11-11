#include <iostream>
#include <queue>
#include <list>
#include <string>
#include <array>
//#include <mutex>
using namespace std;
class QueueRenderer //Singleton
{

private:
	//member variables
	queue<int> sprites; //change string type later to sprite/texture
	//static pointer
	static QueueRenderer* queueRendPtr;

	//MAYBEEE????? use mutex

public:

	QueueRenderer(const QueueRenderer& obj) = delete; //prevent copis
	QueueRenderer();
	void AddToQueue(string spriteName);
	void OrderQueueByZ(queue<int> &sprites);
	void RenderFromQueue();
	void Merger(std::vector<int> arr, int left, int mid, int right);
	void MergeCaller(vector<int> temp, int left, int right);
	void ArrToQueueConverter(std::vector<int> temp, queue<int>& sprites);
	void ClearQueue(queue<int> &sprites);
	static QueueRenderer& Get()
	{
		static QueueRenderer instance;
		return instance;
	}

};