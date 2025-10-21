#include <iostream>
#include <queue>
#include <string>
//#include <mutex>
using namespace std;
class QueueRenderer //Singleton
{

private:
	//member variables
	queue<string>* sprites; //change string type later to sprite/texture
	//static pointer
	static QueueRenderer* queueRendPtr;

	//MAYBEEE????? use mutex

public:

	QueueRenderer(const QueueRenderer& obj) = delete; //prevent copis
	QueueRenderer();
	void AddToQueue(string spriteName);
	void RenderFromQueue();
	void ClearQueue(queue<string> &sprites);
	static QueueRenderer& Get()
	{
		static QueueRenderer instance;
		return instance;
	}

};