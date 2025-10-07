#include "Scripting/Scripting.h"

namespace IonixEngine
{
	Scripting* Scripting::s_Instance = nullptr;

	Scripting& Scripting::Get()
	{
		if (!s_Instance)
			s_Instance = new Scripting();

		return *s_Instance;
	}
}