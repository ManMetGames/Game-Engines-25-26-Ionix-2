#include "NavAgent.h"
#include "Architecture/Application.h"
#include "NavMef.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	void NavAgent::PlaceAgent(b2Vec2 endPosition)
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(entity);
		b2Vec2 position = body->GetPosition();
		int currentCell;
		int goalCell;
		currentCell = m_NavMef->GetPositionInMesh(position);
		goalCell = m_NavMef->GetPositionInMesh(endPosition);
		std::vector<int> path;
		path = m_NavMef->FindPath(position, endPosition);
	}
}