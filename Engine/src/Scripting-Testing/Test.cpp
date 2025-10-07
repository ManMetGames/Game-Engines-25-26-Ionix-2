#include "Scripting-Testing/Test.h"
namespace IonixEngine
{

	Test::Test() {
		Scripting::Get().Init();
		Scripting::Get().ExecuteScript("test.lua");
	};

}
	
		
