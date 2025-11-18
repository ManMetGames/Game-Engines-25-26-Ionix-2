#include "Scripting/Fysics/FysicsScripting.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Fysics/joints.h"
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
		//------------Box2D Vec2 Binding--------------- VERY USEFUL! Can use this in many methods below.
		lua.new_usertype<b2Vec2>("Vec2",
			sol::constructors<b2Vec2(), b2Vec2(float, float)>(),
			"x", &b2Vec2::x,
			"y", &b2Vec2::y
		);


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

		auto setFysicsIsBullet = [](Entity* entity, bool flag) {
			entity->GetComponent<FysicsBody>()->SetIsBullet(entity, flag);
			};

		auto GetGravityScale = [](Entity* entity) -> float {
			return entity->GetComponent<FysicsBody>()->GetGravityScale(entity);
			};
		auto SetGravityScale = [](Entity* entity, float gravityScale) {
			entity->GetComponent<FysicsBody>()->SetGravityScale(entity, gravityScale);
			};



		//-----------Force Methods----------
		auto addFysicsForce = [](Entity* entity, int impulseX, int impulseY, int forcePosX, int forcePosY) {

			b2Vec2 force; force.x = impulseX; force.y = impulseY;
			b2Vec2 origin; origin.x = forcePosX; origin.y = forcePosY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForce(entity, force, origin);
			};

		auto addFysicsForceToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 origin; origin.x = forceX; origin.y = forceY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, origin);
			};

		auto addFysicsAddImpulse = [](Entity* entity, int impulseX, int impulseY, int forcePosX, int forcePosY) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulseX, impulseY, forcePosX, forcePosY);
			};

		auto addFysicsAddImpulseToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 force; force.x = forceX; force.y = forceY;
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulseToCenter(entity, force);
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


		//----------Joints Methods----------
		auto addPrismaticJoint = [](Entity* entityA, Entity* entityB) {
			return Application::Get().layerFysics->GetFysicsManager()->GetPrismaticJoint();
			};
		auto addWeldJoint = [](Entity* entityA, Entity* entityB) {
			return Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint();
			};
		auto addPulleyJoint = [](Entity* entityA, Entity* entityB) {
			return Application::Get().layerFysics->GetFysicsManager()->GetPulleyJoint();
			};
		auto addRevoluteJoint = [](Entity* entityA, Entity* entityB) {
			return Application::Get().layerFysics->GetFysicsManager()->GetRevoluteJoint();
			};
		auto addDistanceJoint = [](Entity* entityA, Entity* entityB) {
			return Application::Get().layerFysics->GetFysicsManager()->GetDistanceJoint();
			};









		//----------Collision Methods----------

		auto addBoxCollider = [](Entity* entity, float sizeX, float sizeY, int offsetX, int offsetY, float angle, bool isTrigger) {

			b2Vec2 size;
			size.x = sizeX;
			size.y = sizeY;

			b2Vec2 offset;
			offset.x = offsetX;
			offset.y = offsetY;

			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddBox(entity, size, offset, angle, isTrigger);
			};











		lua["Fysics"] = lua.create_table_with(
			"get_pos", getFysicsPos,
			"set_pos", setFysicsPos,
			"get_angle", getFysicsAngle,
			"set_angle", setFysicsAngle,
			"get_linear_velocity", getFysicsLinearVelocity,
			"set_linear_velocity", setFysicsLinearVelocity,
			"get_angular_velocity", getFysicsAngularVelocity,
			"set_angular_velocity", setFysicsAngularVelocity,
			"get_awake", getFysicsAwake,
			"set_awake", setFysicsAwake,
			"rotate_pos", rotateFysicsPosition,
			"get_linear_damping", getFysicsLinearDamping,
			"set_linear_damping", setFysicsLinearDamping,
			"get_angular_damping", getFysicsAngularDamping,
			"set_angular_damping", setFysicsAngularDamping,
			"get_allow_sleep", getFysicsAllowSleep,
			"set_allow_sleep", setFysicsAllowSleep,
			"get_fixed_rotation", getFysicsFixedRotation,
			"set_fixed_rotation", setFysicsFixedRotation,
			"get_is_bullet", getFysicsIsBullet,
			"set_is_bullet", setFysicsIsBullet,
			"add_force", addFysicsForce,
			"add_force_to_center", addFysicsForceToCenter,
			"add_impulse", addFysicsAddImpulse,
			"add_impulse_to_center", addFysicsAddImpulseToCenter,
			"add_torque", addFysicsTorque,
			"add_angular_impulse", addFysicsAngularImpulse,
			"clear_forces", clearFysicsForces,
			"get_gravity_scale", GetGravityScale,
			"set_gravity_scale", SetGravityScale,
			"add_box_collider", addBoxCollider,
			"get_prismatic_joint", addPrismaticJoint,
			"get_weld_joint", addWeldJoint,
			"get_pulley_joint", addPulleyJoint,
			"get_revolute_joint", addRevoluteJoint,
			"get_distance_joint", addDistanceJoint,
			"add_box_collider", addBoxCollider
		);
	}
}


