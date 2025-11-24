#pragma once
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Force.h"
#include "Fysics/Joints.h"
#include "Fysics/RigidBodyTransform.h"
#include "Fysics/CollisionListener.h"
#include "Architecture/ECS/Entity.hpp"
#include "FysicsMaterials/FysicsMaterialComponent.h"

#include <unordered_map>
#include "box2d.h"

namespace IonixEngine
{
    class FysicsManager
    {
    private:
 
        b2World* world;
        CollisionListener* collisionListener;
        FysicsShapes* shapes;
        Force* force;
        PrismaticJoints* prismaticJoint;
        WeldJoints* weldJoint;
        PulleyJoints* pulleyJoint;
        RevoluteJoints* revoluteJoint;
        DistanceJoints* distanceJoint;
        FysicsMaterialComponent* material;
        std::unordered_map<b2Body*, Entity*> bodyEntityMap;
        std::unordered_map<b2Body*, RigidBodyTransform> transformMap; //used for interpolation

    public:
        static FysicsManager* GetManager();
        FysicsManager();
        ~FysicsManager();

        FysicsShapes* GetShapes() { return shapes; }
        Force* GetForce() { return force; }
        FysicsMaterialComponent* GetMaterial() { return material; }

        PrismaticJoints* GetPrismaticJoint() { return prismaticJoint;}
        WeldJoints* GetWeldJoint() { return weldJoint;}
        PulleyJoints* GetPulleyJoint() { return pulleyJoint;}
        RevoluteJoints* GetRevoluteJoint() { return revoluteJoint;}
        DistanceJoints* GetDistanceJoint() { return distanceJoint;}

        b2World* GetWorld() { return world; }
        std::unordered_map<b2Body*, Entity*>& GetBodyMap() { return bodyEntityMap; }
        std::unordered_map<b2Body*, RigidBodyTransform>& GetTransformMap() { return transformMap; } //used for interpolation
        
        b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* body);

        void AddEntityBodyPair(Entity* entity, b2Body* body);
        std::vector<b2Vec2> ConvertToWorldSpace(Entity* entity, const std::vector<b2Vec2>& localPositions);
    };
}