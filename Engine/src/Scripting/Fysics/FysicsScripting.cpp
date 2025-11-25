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

		auto setFysicsPosv = [](Entity* entity, b2Vec2 vec2) {
			entity->GetComponent<FysicsBody>()->SetPosition(entity, vec2.x, vec2.y);
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

		auto setFysicsLinearVelocityv = [](Entity* entity, b2Vec2 vec2) {
			entity->GetComponent<FysicsBody>()->SetLinearVelocity(entity, vec2.x, vec2.y);
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

			b2Vec2 force; 
			force.x = impulseX;
			force.y = impulseY;
			b2Vec2 origin; 
			origin.x = forcePosX; 
			origin.y = forcePosY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForce(entity, force, origin);
			};

		auto addFysicsForcev = [](Entity* entity, b2Vec2 impulseVec2, b2Vec2 originVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForce(entity, impulseVec2, originVec2);
			};

		auto addFysicsForceToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 origin; 
			
			origin.x = forceX; 
			origin.y = forceY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, origin);
			};

		auto addFysicsForceToCenterv = [](Entity* entity, b2Vec2 originVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, originVec2);
			};

		auto addFysicsAddImpulse = [](Entity* entity, int impulseX, int impulseY, int forcePosX, int forcePosY) {
			
			b2Vec2 impulse;
			impulse.x = impulseX;
			impulse.y = impulseY;
			b2Vec2 forcePos;
			forcePos.x = forcePosX;
			forcePos.y = forcePosY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulse, forcePos);
			};

		auto addFysicsAddImpulsev = [](Entity* entity, b2Vec2 impulseVec2, b2Vec2 forceVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulseVec2, forceVec2);
			};

		auto addFysicsAddImpulseToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 force; 
			force.x = forceX; 
			force.y = forceY;
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulseToCenter(entity, force);
			};

		auto addFysicsAddImpulseToCenterv = [](Entity* entity, b2Vec2 forceVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulseToCenter(entity, forceVec2);
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

		auto addBoxColliderv = [](Entity* entity, b2Vec2 size, b2Vec2 offset, float angle, bool isTrigger) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddBox(entity, size, offset, angle, isTrigger);
			};

		auto addCircle = [](Entity* entity, float radius, int offsetX, int offsetY, bool isTrigger) {

			b2Vec2 offset;
			offset.x = offsetX;
			offset.y = offsetY;

			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddCircle(entity, radius, offset, isTrigger);

			};

		auto addCirclev = [](Entity* entity, float radius, b2Vec2 offset, bool isTrigger) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddCircle(entity, radius, offset, isTrigger);
			};

		auto addPolygon = [](Entity* entity) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddPolygon(entity);
			};


		lua["Fysics"] = lua.create_table_with(
			"add_circle_collider",addCircle,
			"add_box_collider",	addBoxCollider,
			"add_circle_collider_v", addCirclev,
			"add_box_collider_v", addBoxColliderv,
			"add_polygon_collider",addPolygon,
			"get_pos", getFysicsPos,
			"set_pos", setFysicsPos,
			"set_pos_v", setFysicsPosv,
			"get_angle", getFysicsAngle,
			"set_angle", setFysicsAngle,
			"get_linear_velocity", getFysicsLinearVelocity,
			"set_linear_velocity", setFysicsLinearVelocity,
			"set_linear_velocity_v", setFysicsLinearVelocityv,
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
			"add_force_v", addFysicsForcev,
			"add_force_to_center_v", addFysicsForceToCenterv,
			"add_impulse_v", addFysicsAddImpulsev,
			"add_impulse_to_center_v", addFysicsAddImpulseToCenterv,
			"add_torque", addFysicsTorque,
			"add_angular_impulse", addFysicsAngularImpulse,
			"clear_forces", clearFysicsForces,
			"get_gravity_scale", GetGravityScale,
			"set_gravity_scale", SetGravityScale,
			"get_prismatic_joint", addPrismaticJoint,
			"get_weld_joint", addWeldJoint,
			"get_pulley_joint", addPulleyJoint,
			"get_revolute_joint", addRevoluteJoint,
			"get_distance_joint", addDistanceJoint,
			"add_box_collider", addBoxCollider
		);
	}
}


