#include "NavAgent.h"
#include "Architecture/Application.h"
#include "NavMef.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"
//#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	NavAgent::NavAgent(NavMef* nav, Entity* ent, float speed) : m_NavMef(nav), m_entity(ent), m_speed(speed) {}

	void NavAgent::PlaceAgent(b2Vec2 endPosition) // this is currently a bit wrong because dont have a funell yet 
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);
		b2Vec2 position = body->GetPosition();
		int currentCell;
		int goalCell;
		currentCell = m_NavMef->GetPositionInMesh(position);
		goalCell = m_NavMef->GetPositionInMesh(endPosition);
		if (currentCell == -1 || goalCell == -1) {
			m_path.clear();
			return;
		}
		std::vector<int> path = m_NavMef->FindPath(currentCell, goalCell);
		m_path = m_NavMef->Funnel(path);

		// Incorrect args passed to FindPath - commented out
		//path = m_NavMef->FindPath(position, endPosition);
		m_path.push_back(endPosition);
		m_pathIndex = 0;
	}
	void NavAgent::Update(float dt)
	{
		if (m_path.empty()) return;

		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);

		b2Vec2 target = m_path[m_pathIndex];

		b2Vec2 toTarget = target - body->GetPosition();
		float distance = toTarget.Length();

		if (distance < 0.1f)
		{
			m_pathIndex++;
			if (m_pathIndex >= m_path.size()) {
				// reached the end
				return;
			}
			return;
		}

		toTarget.Normalize();
		body->SetLinearVelocity(m_speed * toTarget);
		if (m_pathIndex >= m_path.size()) {
			body->SetLinearVelocity(b2Vec2_zero);
			return;
		}
	}
}