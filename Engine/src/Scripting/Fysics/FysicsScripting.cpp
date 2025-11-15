#include "Scripting/Fysics/FysicsScripting.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
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

		//------------Fysics Body Methods---------------
		auto getFysicsPos = [](Entity* entity) -> b2Vec2 {
			return entity->GetComponent<FysicsBody>()->GetPosition(entity);
			};

		auto setFysicsPos = [](Entity* entity, float x, float y) {
			entity->GetComponent<FysicsBody>()->SetPosition(entity, x, y);
			};

		auto getFysicsAngle = [](Entity* entity) -> float{
			return entity->GetComponent<FysicsBody>()->GetAngle(entity);
			};

		auto setFysicsAngle = [](Entity* entity, float radians) {
			entity->GetComponent<FysicsBody>()->SetAngle(entity, radians);
			};

		auto getFysicsLinearVelocity = [](Entity* entity) -> b2Vec2{
			return entity->GetComponent<FysicsBody>()->GetLinearVelocity(entity);
			};

		auto setFysicsLinearVelocity = [](Entity* entity, float x, float y) {
			entity->GetComponent<FysicsBody>()->SetLinearVelocity(entity, x, y);
			};

		auto getFysicsAngularVelocity = [](Entity* entity) -> float {
			return entity->GetComponent<FysicsBody>()->GetAngularVelocity(entity);
			};

		auto setFysicsAngularVelocity = [](Entity* entity, float x) {
			entity->GetComponent<FysicsBody>()->SetAngularVelocity(entity, x);
			};

		auto getFysicsAwake = [](Entity* entity) -> bool {
			return entity->GetComponent<FysicsBody>()->GetAwake(entity);
			};

		auto setFysicsAwake = [](Entity* entity, bool flag) {
			entity->GetComponent<FysicsBody>()->SetAwake(entity, flag);
			};

		auto rotateFysicsPosition = [](Entity* entity, float angle) {
			entity->GetComponent<FysicsBody>()->RotatePosition(entity, angle);
			};

		auto getFysicsLinearDamping = [](Entity* entity) -> float{
			return entity->GetComponent<FysicsBody>()->GetLinearDamping(entity);
			};

		auto setFysicsLinearDamping = [](Entity* entity, float damping) {
			entity->GetComponent<FysicsBody>()->SetLinearDamping(entity, damping);
			};

		auto getFysicsAngularDamping = [](Entity* entity) -> float{
			return entity->GetComponent<FysicsBody>()->GetAngularDamping(entity);
			};

		auto setFysicsAngularDamping = [](Entity* entity, float damping) {
			entity->GetComponent<FysicsBody>()->SetAngularDamping(entity, damping);
			};

		auto getFysicsAllowSleep = [](Entity* entity) -> bool {
			return entity->GetComponent<FysicsBody>()->GetAllowSleep(entity);
			};

		auto setFysicsAllowSleep = [](Entity* entity, bool flag) {
			entity->GetComponent<FysicsBody>()->SetAllowSleep(entity, flag);
			};

		auto getFysicsFixedRotation = [](Entity* entity) -> bool {
			return entity->GetComponent<FysicsBody>()->GetFixedRotation(entity);
			};

		auto setFysicsFixedRotation = [](Entity* entity, bool flag) {
			entity->GetComponent<FysicsBody>()->SetFixedRotation(entity, flag);
			};

		auto getFysicsIsBullet = [](Entity* entity) -> bool {
			return entity->GetComponent<FysicsBody>()->GetIsBullet(entity);
			};

		auto setFysicsIsBulletp = [](Entity* entity, bool flag) {
			entity->GetComponent<FysicsBody>()->SetIsBullet(entity, flag);
			};



		//-----------Force Methods----------
		auto addFysicsForce = [](Entity* entity, b2Vec2 force, b2Vec2 point) {		
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForce(entity, force, point);
			};

		auto addFysicsForceToCenter = [](Entity* entity, b2Vec2 force) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, force);
			};

		auto addFysicsAddImpulse = [](Entity* entity, b2Vec2 impulse, b2Vec2 point) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulse, point);
			};

		auto addFysicsAddImpulseToCenter = [](Entity* entity, b2Vec2 impulse) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulseToCenter(entity, impulse);
			};

		auto addFysicsTorque = [](Entity* entity, float torque) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddTorque(entity, torque);
			};

		auto addFysicsAngularImpulse = [](Entity* entity, float torque) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddAngularImpulse(entity, torque);
			};

		auto clearFysicsForces = [](Entity* entity) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->ClearForces(entity);
			};



		//----------Collision Methods----------




	}
}


