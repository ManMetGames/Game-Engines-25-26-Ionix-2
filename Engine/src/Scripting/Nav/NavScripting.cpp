
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

		auto drawNavGrid = [](float x, float y, float width, float height, float cellSize)
		{
			// Visualize the grid
			SDL_Color gridColor = { 255, 0, 0, 20 }; // red
			Application::Get().layerGraphics->GetQueue()->DrawGrid(x, y, width, height, cellSize, gridColor);
		};

		auto addNavObstacle = [](Entity* entity)
		{
			
				Application::Get().layerNavigation->GetNavMef()->AddObstacle(entity);
		};
		
		auto createNavAgent = [](Entity* entity, float speed) -> NavAgent*
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
			"draw_nav_grid", drawNavGrid,
			"add_obstacle", addNavObstacle,
			"create_agent", createNavAgent,
			"agent_move_to", navAgentMoveTo
		);
	}
}


