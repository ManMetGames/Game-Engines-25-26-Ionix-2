#include <map>
#include <string>
#include "SDL.h"
#include "EventSystem/Event.h"
#include <unordered_map>
#include "LayerSystem/Layer.h"

using namespace std;
namespace IonixEngine
{
	class LayerInput : public Layer
	{
	public:
		LayerInput() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(IonixEvent& e) override;

		void AddKey(SDL_Keycode k, bool b) { keyMap.insert({ k, b }); }

		std::unordered_map<SDL_Keycode, bool> keyMap;
	};
}
