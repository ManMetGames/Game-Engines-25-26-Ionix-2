#include <iostream>
#include <queue>
#include <list>
#include <string>
#include <array>
//#include <mutex>
using namespace std;

namespace IonixEngine {

	struct RenderCall { //render data
		SDL_Texture* texture;
		// SDL_Rect src;
		SDL_Rect dest;
		int32_t z;
	};

	class QueueRenderer //Singleton
	{

	private:
		//member variables
		queue<int>* sprites; //change string type later to sprite/texture
		//static pointer
		static QueueRenderer* queueRendPtr;

		//MAYBEEE????? use mutex

	public:

		QueueRenderer(const QueueRenderer& obj) = delete; //prevent copis
		QueueRenderer();
		void AddToQueue(string spriteName);
		void OrderQueueByZ(queue<int>& sprites);
		void RenderFromQueue();
		void ClearQueue(queue<int>& sprites);
		static QueueRenderer& Get()
		{
			static QueueRenderer instance;
			return instance;
		}

		//sort algorithm stuff
		void Merger(vector<int> arr, int left, int mid, int right);
		void MergeCaller(queue<int>& sprites, vector<int> arr, int left, int right);
		void ArrToQueueConverter(vector<int> temp, queue<int>& sprites);

	};
}