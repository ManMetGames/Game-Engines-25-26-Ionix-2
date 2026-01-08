#include "NavAgent.h"
#include "Architecture/Application.h"
#include "NavMef.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"

namespace IonixEngine
{
	NavAgent::NavAgent(NavMef* nav, Entity* ent, float speed)
		: m_NavMef(nav), m_entity(ent), m_speed(speed) {
	}

	inline b2Vec2 EvaluateBezier(const b2Vec2& p0, const b2Vec2& p1, const b2Vec2& p2, const b2Vec2& p3, float t)
	{
		float u = 1.0f - t;
		float uu = u * u;
		float tt = t * t;
		float uuu = uu * u;
		float ttt = tt * t;

		b2Vec2 point = b2Vec2_zero;
		point.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
		point.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;

		return point;
	}

	std::vector<BezierSegment> NavAgent::SmoothPath(const std::vector<b2Vec2>& path, float smoothFactor)
	{
		std::vector<BezierSegment> segments;

		if (path.size() < 3)
		{
			return segments;
		}

		for (int i = 1; i < path.size() - 1; i++)
		{
			b2Vec2 prev = path[i - 1];
			b2Vec2 current = path[i];
			b2Vec2 next = path[i + 1];

			b2Vec2 dirIn = b2Vec2Normalize(current - prev);
			b2Vec2 dirOut = b2Vec2Normalize(next - current);

			float distIn = b2Distance(prev, current);
			float distOut = b2Distance(current, next);

			b2Vec2 p0 = current - b2Vec2(dirIn.x * distIn * smoothFactor,dirIn.y * distIn * smoothFactor);
			b2Vec2 p3 = current + b2Vec2(dirOut.x * distOut * smoothFactor,dirOut.y * distOut * smoothFactor);
			b2Vec2 p1 = b2Lerp(p0, current, 0.5f);
			b2Vec2 p2 = b2Lerp(current, p3, 0.5f);

			segments.push_back({ p0, p1, p2, p3 });
		}
		return segments;
	}

	void NavAgent::PlaceAgent(b2Vec2 endPosition)
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);

		b2Vec2 agentCentre = body->GetWorldCenter();

		int currentCell = m_NavMef->GetPositionInMesh(agentCentre);
		int goalCell = m_NavMef->GetPositionInMesh(endPosition);

		if (currentCell == -1 || goalCell == -1)
		{
			body->SetLinearVelocity(b2Vec2_zero);
			m_path.clear();
			return;
		}

		std::vector<int> path = m_NavMef->FindPath(currentCell, goalCell);
		m_path = m_NavMef->Funnel(path);
		m_segments = SmoothPath(m_path, 0.3f);
		
		if (m_segments.empty())
		{
			body->SetLinearVelocity(b2Vec2_zero);
			return;
		}

		m_path[0] = agentCentre;
		m_path.back() = endPosition;

		m_pathIndex = 0;
	}
	inline b2Vec2 EvaluateBezier(const BezierSegment& s, float t)
	{
		float u = 1.0f - t;
		float uu = u * u;
		float tt = t * t;
		float uuu = uu * u;
		float ttt = tt * t;

		return b2Vec2(
			uuu * s.p0.x + 3 * uu * t * s.p1.x + 3 * u * tt * s.p2.x + ttt * s.p3.x,
			uuu * s.p0.y + 3 * uu * t * s.p1.y + 3 * u * tt * s.p2.y + ttt * s.p3.y
		);
	}

	void NavAgent::Update(float dt)
	{
		b2Body* body = FysicsManager::GetManager()->GetBodyFromEntity(m_entity);

		if (m_segments.empty())
		{
			body->SetLinearVelocity(b2Vec2_zero);
			return;
		}

		m_t += dt * (m_speed * 0.25f); // adjust 0.25f to tune speed along curve
		if (m_t > 1.0f)
		{
			m_t = 0.0f;
			m_segmentIndex++;

			if (m_segmentIndex >= m_segments.size())
			{
				body->SetLinearVelocity(b2Vec2_zero);
				m_segments.clear();
				return;
			}
		}

		const BezierSegment& seg = m_segments[m_segmentIndex];
		b2Vec2 target = EvaluateBezier(seg, m_t);

		// 3️⃣ Move toward it
		b2Vec2 agentCentre = body->GetWorldCenter();
		b2Vec2 toTarget = target - agentCentre;
		toTarget.Normalize();
		body->SetLinearVelocity(b2Vec2(toTarget.x * m_speed, toTarget.y * m_speed));
	}

	void NavAgent::MoveTo(const b2Vec2& endPosition)
	{
		PlaceAgent(endPosition);
	}
}
