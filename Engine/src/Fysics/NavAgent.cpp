#include "NavAgent.h"
#include "Architecture/Application.h"
#include "NavMef.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	NavAgent::NavAgent(NavMef* nav, Entity* ent, float speed) : m_NavMef(nav), m_entity(ent), m_speed(speed) {}

	void NavAgent::PlaceAgent(b2Vec2 endPosition) 
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);
		b2Vec2 position = body->GetPosition();
		int currentCell;
		int goalCell;
		currentCell = m_NavMef->GetPositionInMesh(position);
		goalCell = m_NavMef->GetPositionInMesh(endPosition);
		if (currentCell == -1 || goalCell == -1) {
			body->SetLinearVelocity(b2Vec2_zero);
			m_path.clear();
			return;
		}
		std::vector<int> path = m_NavMef->FindPath(currentCell, goalCell);
		m_path = m_NavMef->Funnel(path);
		if (m_path.empty())
		{
			body->SetLinearVelocity(b2Vec2_zero);
			return;
		}
		m_path[0] = body->GetPosition();

		m_path.back() = endPosition;
		std::cout << "Agent pos: "
			<< position.x << ", " << position.y << std::endl;

		std::cout << "Target pos: "
			<< endPosition.x << ", " << endPosition.y << std::endl;

		std::cout << "Start cell: " << currentCell
			<< " Goal cell: " << goalCell << std::endl;

		// Incorrect args passed to FindPath - commented out
		//path = m_NavMef->FindPath(position, endPosition);
		m_pathIndex = 0;
	}
	void NavAgent::Update(float dt)
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);

		if (m_path.empty())
		{
			body->SetLinearVelocity(b2Vec2_zero);
			return;
		}

		b2Vec2 target = m_path[m_pathIndex];
		b2Vec2 toTarget = target - body->GetPosition();
		float distance = toTarget.Length();

		const float arriveRadius = 0.25f;

		if (distance <= arriveRadius)
		{
			body->SetTransform(target, body->GetAngle());
			m_pathIndex++;

			if (m_pathIndex >= m_path.size())
			{
				body->SetLinearVelocity(b2Vec2_zero);
				m_path.clear();
			}

			return;
		}

		toTarget.Normalize();
		body->SetLinearVelocity(m_speed * toTarget);
	}

	void NavAgent::MoveTo(const b2Vec2& endPosition)
	{
		PlaceAgent(endPosition);
	}
}