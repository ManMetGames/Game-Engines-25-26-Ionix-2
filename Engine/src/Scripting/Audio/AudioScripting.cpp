#include "Scripting/Audio/AudioScripting.h"

namespace IonixEngine
{
	AudioScripting* AudioScripting::s_Instance = nullptr;
	AudioScripting& AudioScripting::Get() {
		if (!s_Instance) {
			s_Instance = new AudioScripting();
		}
		return *s_Instance;
	}
	void AudioScripting::Init(sol::state& sharedState)
	{
		if (m_LuaState == &sharedState) {
			std::cout << "Lua Audio Scripting already initialized on this state.\n";
			return;
		}

		std::cout << "Lua Audio Scripting is now being initialized." << std::endl;
		m_LuaState = &sharedState;
		RegisterAudioBindings();
		std::cout << "Lua Audio Scripting has been initialised successfully." << std::endl;
	}


	void AudioScripting::RegisterAudioBindings()
	{
		sol::state& lua = *m_LuaState; 

		lua.new_usertype<Audio>("Audio",
			sol::constructors<Audio(const std::string&, const std::string&)>(),
			"Play", &Audio::Play,
			"SetVolume", &Audio::SetVolume,
			"Loop", &Audio::Loop,
			"PauseAll", &Audio::PauseAll,
			"ResumeAll", &Audio::ResumeAll
		);
	}
}

