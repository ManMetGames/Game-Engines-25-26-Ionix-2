#include "Scripting/Fysics/FysicsScripting.h"
#include "Architecture/Application.h"
namespace IonixEngine
{
	FysicsScripting* FysicsScripting::s_Instance = nullptr;

	FysicsScripting& FysicsScripting::Get() {
		if (!s_Instance)
			s_Instance = new FysicsScripting();
		return *s_Instance;
	}

	void FysicsScripting::Init(sol::state& lua)
	{
		
	}
}


