
#include "Scripting/Nav/NavScripting.h"
#include "Architecture/Application.h"
#include "Fysics/NavMef.h"
#include "Fysics/NavAgent.h"
namespace IonixEngine
{
	NavScripting* NavScripting::s_Instance = nullptr;

	NavScripting& NavScripting::Get() {
		if (!s_Instance)
			s_Instance = new NavScripting();
		return *s_Instance;
	}

	void NavScripting::Init(sol::state& lua)
	{

		//------------------------Nav Mef----------------------------
		auto loadNavMef = [](float x, float y, float width, float height,  float cellSize)
		{
			
			
			Application::Get().layerNavigation->GetNavMef()->BuildGrid({ x, y }, { width, height }, cellSize);
		};

		auto addNavObstacle = [](float minX, float minY, float maxX, float maxY)
		{
				Application::Get().layerNavigation->GetNavMef()->AddObstacle({ minX, minY }, { maxX, maxY });
		};
		
		auto createNavAgent = [](Entity* entity, float speed)
		{
				return Application::Get().layerNavigation->CreateAgent(entity, speed);
		};
		auto navAgentMoveTo = [](NavAgent* agent, float x, float y)
		{
			if (!agent) return;
			agent->MoveTo({ x, y });
		};

		lua["Nav"] = lua.create_table_with(
			"load_nav_mef", loadNavMef,
			"add_obstacle", addNavObstacle,
			"create_agent", createNavAgent
		);
		lua.new_usertype<NavAgent>(
			"NavAgent",
			"move_to", navAgentMoveTo
		);
	}
}


