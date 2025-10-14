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
		auto& lua = m_LuaState;

		SoundManager& soundManager = SoundManager::GetInstance();

		lua["Audio"] = lua.create_table_with(
			"load_sound", [](const std::string& name, const std::string& filePath) {
				return SoundManager::GetInstance().LoadSound(name, filePath);
			},
			"play_sound", [](const std::string& name, int loops = 0) {
				SoundManager::GetInstance().PlaySound(name, loops);
			},
			"set_volume", [](const std::string& name, float volume) {
				SoundManager::GetInstance().SetVolume(name, volume);
			}
		);

		std::cout << "[Lua] Audio bindings registered." << std::endl;
	}
}

