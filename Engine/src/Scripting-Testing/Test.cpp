#include "Scripting-Testing/Test.h"
namespace IonixEngine
{

	Test::Test() {
		Scripting::Get().Init();
		Scripting::Get().GetLuaState().script_file("settings.lua");
	};

}
	
		
