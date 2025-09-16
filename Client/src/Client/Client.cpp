// Links to and runs our Engine.dll project in an executable window
#include <EngineIncludes.h>

namespace ClientApp
{
	class Client : public IonixEngine::Application
	{
	public:
		Client() = default;
		~Client() = default;
	};

	IonixEngine::Application* CreateApplication()
	{
		return new Client();
	}
}

