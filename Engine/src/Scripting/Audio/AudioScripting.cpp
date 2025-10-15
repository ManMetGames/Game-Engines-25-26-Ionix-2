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
	void AudioScripting::Init() {
		std::cout << "Lua Audio Scripting is now being initialized." << std::endl;
		m_LuaState.open_libraries(
			sol::lib::base,
			sol::lib::string,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);
		RegisterAudioBindings();
		std::cout << "Lua Audio Scripting has been initialised successfully." << std::endl;
	}
	void AudioScripting::RegisterAudioBindings()
	{
		sol::state& lua = m_LuaState;

		lua.new_usertype<Audio>("Audio",
			sol::constructors<Audio(const std::string&, const std::string&)>(),
			"Play", &Audio::Play,
			"SetVolume", &Audio::SetVolume,
			"Loop", &Audio::Loop,
			"PauseAll", &Audio::PauseAll,
			"ResumeAll", &Audio::ResumeAll
		);

		std::cout << "[Lua] Audio class registered with SoundManager backend.\n";
	}
}

