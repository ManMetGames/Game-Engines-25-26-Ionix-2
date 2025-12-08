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

		lua.new_usertype<RayHit>("RayHit",
			sol::constructors<RayHit()>(),
			"entity", &RayHit::entity
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

		auto getFysicsGravityScale = [](Entity* entity) -> float {
			return entity->GetComponent<FysicsBody>()->GetGravityScale(entity);
			};

		auto setFysicsGravityScale = [](Entity* entity, float gravityScale) {
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

		//Material Changes
		
		auto fysicsUpdateMaterialProperties = [](Entity* entity, float friction, float restitution) {
			Application::Get().layerFysics->GetFysicsManager()->GetMaterial()->UpdateMaterial(entity, friction, restitution);
		};

		auto getFriction = [](Entity* entity)-> float {
			return Application::Get().layerFysics->GetFysicsManager()->GetMaterial()->GetFriction(entity);
		};

		auto getRestitution = [](Entity* entity)-> float {
			return Application::Get().layerFysics->GetFysicsManager()->GetMaterial()->GetRestitution(entity);
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
		auto addSpriteCollider = [](Entity* entity, bool isTrigger, float scaleFactor) {
			if (scaleFactor == 0)
			{
				scaleFactor = 1.0f;
			}
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddSpriteCollider(entity, isTrigger, scaleFactor);
		};
		

		auto addPolygonCollider = [](Entity* entity, float tileSize, std::vector<b2Vec2>& terrainPositions) {
			for (int i = 0; i < terrainPositions.size(); i++)
			{
				terrainPositions[i] = tileSize * b2Vec2 {terrainPositions[i]};
			}
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddPolygon(entity, tileSize, terrainPositions);
		};

		auto addCircleCollider = [](Entity* entity, float radius, int offsetX, int offsetY, bool isTrigger)
		{
			b2Vec2 offset;
			offset.x = offsetX;
			offset.y = offsetY;
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddCircle(entity, radius, offset, isTrigger);
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
		auto setDistanceJoint = [](Entity* entityA, Entity* entityB, float length) {
			Application::Get().layerFysics->GetFysicsManager()->GetDistanceJoint()->setJoint(entityA, entityB, length);
		};
		auto destroyJoint = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i <jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->destroyJoint(jointList);
		};

		auto getBodyA = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getBodyA(jointList);
		};

		auto getBodyB = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getBodyB(jointList);
		};
		
		auto getAnchorA = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getAnchorA(jointList);
		};

		auto getAnchorB = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getAnchorB(jointList);
		};

		auto getUserData = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getUserData(jointList);
		};

		auto getReactionForce = [](int jointID, int inv_dt) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getReactionForce(jointList, inv_dt);
		};

		auto getReactionTorque = [](int jointID, int inv_dt) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getReactionTorque(jointList, inv_dt);
		};

		auto isEnabled = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0;	 i < jointID; i++) {
				jointList->GetNext();
			}
			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->isEnabled(jointList);
		};

		auto shfitOrigin = [](int jointID, float originX, float originY) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}

			b2Vec2 origin;
			origin.x = originX;
			origin.y = originY;

			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->shiftOrigin(jointList, origin);
		};

		//Weld Joints

		auto getDampingFromWeldJoint = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}		

			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getDamping((b2WeldJoint*)jointList);
		};

		auto setDampingFromWeldJoint = [](int jointID, float newDamping) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}

			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->setDamping((b2WeldJoint*)jointList, newDamping);
		};

		auto getStiffnessFromWeldJoint = [](int jointID) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}

			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->getStiffness((b2WeldJoint*)jointList);
		};

		auto setStiffnessFromWeldJoint = [](int jointID, float newStiffness) {
			if (Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointCount() <= 0) { return; }
			b2Joint* jointList = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->GetJointList();
			for (int i = 0; i < jointID; i++) {
				jointList->GetNext();
			}

			Application::Get().layerFysics->GetFysicsManager()->GetWeldJoint()->setStiffness((b2WeldJoint*)jointList, newStiffness);
		};

		//----------EntityMap----------
		auto addToCollisionMap = [](Entity* entityA, Entity* entityB)
		{
			if (entityA->GetComponent<FysicsBody>() && entityB->GetComponent<FysicsBody>())
			{
				Application::Get().layerFysics->GetFysicsManager()->GetCollisionListener()->AddToCollisionMap(entityA, entityB);
			}
		};
		//---------------Raycasting--------
		auto raycast = [](Vec2 startPos, Vec2 endPos)->std::tuple<bool, RayHit>
		{

			b2Vec2 pos1 = b2Vec2(startPos.x, startPos.y);
			b2Vec2 pos2 = b2Vec2(endPos.x, endPos.y);
			RayHit hit;
			bool hitSomething = Application::Get().layerFysics->GetFysicsManager()->GetRaycast()->CastFirst(b2Vec2(pos1.x / 100, pos1.y / 100), pos2, hit);
			if (!hitSomething)
			{
				Application::Get().layerGraphics->GetQueue()->DrawLine(pos1.x, pos1.y, endPos.x, endPos.y, false);

				return std::make_tuple(false, RayHit());
			}
			Application::Get().layerGraphics->GetQueue()->DrawLine(pos1.x, pos1.y, endPos.x, endPos.y, true);

			return std::make_tuple(true, hit);
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
			"get_gravity_scale", getFysicsGravityScale,
			"set_gravity_scale", setFysicsGravityScale,
			"add_force", addFysicsForce,
			"add_force_to_center", addFysicsForceToCenter,
			"add_impulse", addFysicsAddImpulse,
			"add_impulse_to_center", addFysicsAddImpulseToCenter,
			"add_torque", addFysicsTorque,
			"add_angular_impulse", addFysicsAngularImpulse,
			"clear_forces", clearFysicsForces,
			"add_box_collider", addBoxCollider,
			"set_material_properties", fysicsUpdateMaterialProperties,
			"get_friction", getFriction,
			"get_restitution", getRestitution,
			"add_polygon_collider", addPolygonCollider,
			"add_sprite_collider", addSpriteCollider,
			"create_prismatic_joint", setPrismaticJoint,
			"create_weld_joint", setWeldJoint,
			"create_pulley_joint", setPulleyJoint,
			"create_revolute_joint", setRevoluteJoint,
			"create_distance_joint", setDistanceJoint,
			"destroy_joint", destroyJoint,
			"get_bodyA", getBodyA,
			"get_bodyB", getBodyB,
			"get_anchorA", getAnchorA,
			"get_anchorB", getAnchorB,
			"get_userData", getUserData,
			"get_reaction_force", getReactionForce,
			"get_reaction_torque", getReactionTorque,
			"is_enabled", isEnabled,
			"shift_origin", shfitOrigin,
			"get_damping", getDampingFromWeldJoint,
			"set_damping", setDampingFromWeldJoint,
			"get_stiffness", getStiffnessFromWeldJoint,
			"set_stiffness", setStiffnessFromWeldJoint,
			"add_to_collision_map", addToCollisionMap,
			"raycast", raycast
		);
	}
}


