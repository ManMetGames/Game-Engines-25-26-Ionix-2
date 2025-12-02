#include "NavAgent.h"
#include "Architecture/Application.h"
#include "NavMef.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	NavAgent::NavAgent(NavMef* nav, Entity* ent) : m_NavMef(nav), m_entity(ent) {}

	void NavAgent::PlaceAgent(b2Vec2 endPosition)
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);
		b2Vec2 position = body->GetPosition();
		int currentCell;
		int goalCell;
		currentCell = m_NavMef->GetPositionInMesh(position);
		goalCell = m_NavMef->GetPositionInMesh(endPosition);
		std::vector<int> path;

		// Incorrect args passed to FindPath - commented out
		//path = m_NavMef->FindPath(position, endPosition);
		path = m_NavMef->FindPath(currentCell, goalCell);
		m_pathIndex = 0;
	}
	void NavAgent::Update(float dt)
	{
	}
}