#pragma once
#include "sol/sol.hpp"
#include "Scripting/Scripting.h"
#include "LayerSystem/Layers/LayerSound.h"
#include "Architecture/AudioSystem/SoundManager.h"

namespace IonixEngine
{
	class AudioScripting
	{
	public:
		//singleton
		static AudioScripting& Get();
		//Core
		void Init();
		// Registration Methods
		void RegisterAudioBindings();
		void ExecuteScript(const std::string& scriptName);
		void CallHook(const std::string& hookName);
		sol::state& GetLuaState() { return m_LuaState; }
	private:
		//singleton Instance
		static AudioScripting* s_Instance;
		sol::state m_LuaState;
	};
}