#include "Scripting/Scripting.h"

namespace IonixEngine 
{
	Scripting* Scripting::s_Instance = nullptr;

	Scripting& Scripting::Get() 
	{
		if (!s_Instance) 
		{
			s_Instance = new Scripting();
		}

		return *s_Instance;
	}

	void Scripting::Init() 
	{
		std::cout << "Lua is now being Initialized!" << std::endl;

		m_LuaState.open_libraries(
			sol::lib::base,
			sol::lib::package,
			sol::lib::string,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);
	}
}
