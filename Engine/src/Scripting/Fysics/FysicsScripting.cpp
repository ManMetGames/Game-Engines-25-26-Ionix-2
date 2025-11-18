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

		//----------Joint Methods----------

		auto setPrismaticJoint = [](Entity* entityA, Entity* entityB, float worldAxisX, float worldAxisY, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor) {
			b2Vec2 worldAxis;
			worldAxis.x = worldAxisX;
			worldAxis.y = worldAxisY;

			Application::Get().layerFysics->GetFysicsManager()->GetPrismaticJoint()->setJoint(entityA, entityB, worldAxis, lowerTranslation, upperTranslation, enableLimit, maxMotorForce, motorSpeed, enableMotor);
	    };
		auto setWeldJoint = [](Entity* entityA, Entity* entityB) {
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->setJoint(entityA, entityB);
		};
		auto setPulleyJoint = [](Entity* entityA, Entity* entityB, float position1X, float position1Y, float position2X, float position2Y , float ratio, float lengthA, float lengthB) {

			b2Vec2 p1; b2Vec2 p2;
			p1.x = position1X;
			p1.y = position1Y;

			p2.x = position2X;
			p2.y = position2Y;

			Application::Get().layerFysics->GetFysicsManager()->GetPulleyJoint()->setJoint(entityA, entityB, p1, p2, ratio, lengthA, lengthB);
	    };
		auto setRevoluteJoint = [](Entity* entityA, Entity* entityB, bool enableLimit, float lowerAngle, float upperAngle, bool enableMotor, float motorSpeed, float maxMotorTorque) {
			Application::Get().layerFysics->GetFysicsManager()->GetRevoluteJoint()->setJoint(entityA, entityB, enableLimit, lowerAngle, upperAngle, enableMotor, motorSpeed, maxMotorTorque);
		};
		auto setDistanceJoint = [](Entity* entityA, Entity* entityB, float anchorAX, float anchorAY, float anchorBX, float anchorBY, float length) {
			b2Vec2 anchorA; b2Vec2 anchorB;
			anchorA.x = anchorAX;
			anchorA.y = anchorAY;

			anchorB.x = anchorBX;
			anchorB.y = anchorBY;

			Application::Get().layerFysics->GetFysicsManager()->GetDistanceJoint()->setJoint(entityA, entityB, anchorA, anchorB, length);
		};
		auto destroyWeldJoint = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i <jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->destroyJoint(jointList);
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
			"add_box_collider", addBoxCollider,
			"create_prismatic_joint", setPrismaticJoint,
			"create_weld_joint", setWeldJoint,
			"create_pulley_joint", setPulleyJoint,
			"create_revolute_joint", setRevoluteJoint,
			"create_distance_joint", setDistanceJoint,
			"destroy_joint", destroyWeldJoint
	
		);
	}
}


