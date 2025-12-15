
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
		auto getRaycastEntity = [](RayHit hit)->Entity*
		{
			return hit.entity;
		};

		//------------Fysics Body Methods---------------
		auto getFysicsPos = [](Entity* entity) -> b2Vec2 {
			return entity->GetComponent<FysicsBody>()->GetPosition(entity);
			};

		auto setFysicsPos = [](Entity* entity, float x, float y) {
			entity->GetComponent<FysicsBody>()->SetPosition(entity, x / 100, y / 100);
			};

		auto setFysicsPosv = [](Entity* entity, b2Vec2 vec2) {
			entity->GetComponent<FysicsBody>()->SetPosition(entity, vec2.x / 100, vec2.y / 100);
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

		auto addFysicsForcev = [](Entity* entity, b2Vec2 impulseVec2, b2Vec2 originVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForce(entity, impulseVec2, originVec2);
			};

		auto addFysicsForceToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 origin; origin.x = forceX; origin.y = forceY;

			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, origin);
			};

		auto addFysicsForceToCenterv = [](Entity* entity, b2Vec2 originVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddForceToCenter(entity, originVec2);
			};

		auto addFysicsAddImpulse = [](Entity* entity, int impulseX, int impulseY, int forcePosX, int forcePosY) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulseX, impulseY, forcePosX, forcePosY);
			};

		auto addFysicsAddImpulsev = [](Entity* entity, b2Vec2 impulseVec2, b2Vec2 forceVec2) {
			Application::Get().layerFysics->GetFysicsManager()->GetForce()->AddImpulse(entity, impulseVec2.x, impulseVec2.y, forceVec2.x, forceVec2.y);
			};

		auto addFysicsAddImpulseToCenter = [](Entity* entity, int forceX, int forceY) {
			b2Vec2 force; force.x = forceX; force.y = forceY;
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

		auto addBoxColliderv = [](Entity* entity, b2Vec2 size, b2Vec2 offset, float angle, bool isTrigger) {

			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddBox(entity, size, offset, angle, isTrigger);
			};


		auto addPolygonCollider = [](Entity* entity, float tileSize, std::vector<b2Vec2>& terrainPositions) {
			for (int i = 0; i < terrainPositions.size(); i++)
			{
				terrainPositions[i] = tileSize * b2Vec2 {terrainPositions[i]};
			}
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddPolygon(entity, tileSize, terrainPositions);
		};

		auto getColliderWidth = [](Entity* entity) -> float {
			return Application::Get().layerFysics->GetFysicsManager()->GetShapes()->GetWidth();
			};

		auto setColliderWidth = [](Entity* entity, float w, int shapeType) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->SetWidth(w, static_cast<fysicShapeType>(shapeType));
			};

		auto getColliderHeight = [](Entity* entity) -> float {
			return Application::Get().layerFysics->GetFysicsManager()->GetShapes()->GetHeight();
			};

		auto setColliderHeight = [](Entity* entity, float h, int shapeType) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->SetHeight(h, static_cast<fysicShapeType>(shapeType));
			};

		auto isColliderTrigger = [](Entity* entity) -> bool {
			return Application::Get().layerFysics->GetFysicsManager()->GetShapes()->IsShapeTrigger();
			};

		auto setColliderTrigger = [](Entity* entity, bool value) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->SetShapeTrigger(value);
			};

		auto getColliderVertices = [](Entity* entity) {
			return Application::Get().layerFysics->GetFysicsManager()->GetShapes()->GetVertices();
			};

		auto setColliderVertices = [](Entity* entity, std::vector<b2Vec2> verts, int shapeType) {
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->SetVertices(verts, static_cast<fysicShapeType>(shapeType));
			};

		auto getColliderShapeType = [](Entity* entity) {
			return Application::Get().layerFysics->GetFysicsManager()->GetShapes()->GetShapeType();
			};

			
		auto addSpriteCollider = [](Entity* entity, bool isTrigger, float scaleFactor) {
			if (scaleFactor == 0)
			{
				scaleFactor = 1.0f;
			}
			Application::Get().layerFysics->GetFysicsManager()->GetShapes()->AddSpriteCollider(entity, isTrigger, scaleFactor);
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
			for (int i = 0; i < jointID; i++) {
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
			RayHit hit;
			std::cout << endPos.x << ", " << endPos.y << std::endl;
			bool hitSomething = Application::Get().layerFysics->GetFysicsManager()->GetRaycast()->CastFirst(b2Vec2(startPos.x / 100, startPos.y / 100), b2Vec2(endPos.x / 100, endPos.y / 100), hit);
			if (!hitSomething)
			{
				return std::make_tuple(false, RayHit());
			}

			return std::make_tuple(true, hit);
		};

		auto drawRaycast = [](Vec2 startPos, Vec2 endPos, bool hitColor)
		{
			Application::Get().layerGraphics->GetQueue()->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y, hitColor);
		};

		

		auto checkActiveCollisions = [](Entity* entityA, Entity* entityB)->bool
		{
			if (Application::Get().layerFysics->GetFysicsManager()->GetCollisionListener()->CheckActiveCollisions(entityA, entityB))
			{
				return true;
			}

			return false;
		};

		lua["Raycast"] = lua.create_table_with(
					"entity", getRaycastEntity

					);

		lua["Fysics"] = lua.create_table_with(
			"add_box_collider",	addBoxCollider,
			"add_box_collider_v", addBoxColliderv,
			//"add_edge_collider", addEdgeCollider,
			"add_polygon_collider",addPolygonCollider,
			"add_circle_collider", addCircleCollider,
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
			"get_gravity_scale", getFysicsGravityScale,
			"set_gravity_scale", setFysicsGravityScale,
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
			"get_collider_width", getColliderWidth,
			"set_collider_width", setColliderWidth,
			"get_collider_height", getColliderHeight,
			"set_collider_height", setColliderHeight,
			"is_collider_trigger", isColliderTrigger,
			"set_collider_trigger", setColliderTrigger,
			"get_collider_vertices", getColliderVertices,
			"set_collider_vertices", setColliderVertices,
			"get_collider_type", getColliderShapeType,
			"set_material_properties", fysicsUpdateMaterialProperties,
			"get_friction", getFriction,
			"get_restitution", getRestitution,
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
			"col", checkActiveCollisions,
			"add_to_collision_map", addToCollisionMap,
			"raycast", raycast,
			"draw_raycast", drawRaycast
		);
	}
}


