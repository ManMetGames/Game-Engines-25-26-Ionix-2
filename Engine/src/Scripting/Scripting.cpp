#include "Scripting/Scripting.h"
#include "Architecture/Application.h"

namespace IonixEngine {
	Scripting* Scripting::s_Instance = nullptr;

	Scripting& Scripting::Get() {
		if (!s_Instance) {
			s_Instance = new Scripting();
		}
		return *s_Instance;
	}
	void Scripting::Init() {
		std::cout << "Lua is now being initialized." << std::endl;

		m_LuaState.open_libraries(
			sol::lib::base,
			sol::lib::string,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);
		RegisterEngineBindindings();
		std::cout << "Lua has been initialised successfully." << std::endl;
	}

	void Scripting::RegisterEngineBindindings()
	{
		RegisterWindowBindindings();
	}

	void Scripting::ExecuteScript(const std::string& scriptName)
	{
		m_LuaState.script_file(scriptName);
	}

	void Scripting::CallHook(const std::string& hookName)
	{
		sol::function hook = m_LuaState[hookName];
		if (hook.valid()) {
			try {
				hook();
			}
			catch (const std::exception& e) {
				std::cerr << "Error calling hook '" << hookName << "': " << e.what() << '\n';
			}
		}
		else {
			std::cerr << "Hook '" << hookName << "' is invalid\n";
		}
	}

	void Scripting::RegisterWindowBindindings()
	{
		auto getWindowTitle = []()->std::string {

			return Application::Get().GetWindow().m_Data.Title;
			};
		auto getWindowWidth = []() -> std::int32_t {
			return Application::Get().GetWindow().m_Data.Width;
			};
		auto getWindowHeight = []()-> std::int32_t {
			return Application::Get().GetWindow().m_Data.Height;
			};
		m_LuaState["Window"] = m_LuaState.create_table_with(
			"get_title", getWindowTitle,
			"get_width", getWindowWidth,
			"get_height", getWindowHeight
		);

	}// print (Window.get_title())
}