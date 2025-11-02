#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"

#include <unordered_map>
#include <vector>
#include <functional>

#include "Maf/MafUtils.h"

using EntityID = int;

namespace IonixEngine
{
    class FysicsManager
    {
    private:
        FysicsShapes* shape = new FysicsShapes();
        PrismaticJoints* joint;
        Force* force;

        std::unordered_map<int, Joints*> jointDic;
        int jointIncrement = 0;

    public:
        static LayerFysics* s_instance;
        std::unordered_map<int, FysicsBody*> BodyDic;
        int BodyIncrement = 0;

        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void FB_Create()
        {
            FysicsBody* body = new FysicsBody(s_instance->GetWorld());
            BodyDic[BodyIncrement] = body;
            std::cout << "Body Created at Index: " << BodyIncrement << std::endl;
            BodyIncrement++;
        }

        void FB_AddShape(int dicIndex, fysicShapeType shapeType)
        {
            shape->SetBody(BodyDic[dicIndex]->GetBody());
            switch (shapeType)
            {
            case fysicShapeType::circle:
                shape->AddCircle();
                break;
            case fysicShapeType::box:
                shape->AddBox();
                break;
            }
        }

        void FB_AddPolygon(int dicIndex, std::vector<b2Vec2>& vertices)
        {
            shape->SetBody(BodyDic[dicIndex]->GetBody());
            shape->AddPolygon(vertices);
        }

        void FB_Destroy(int dicIndex)
        {
            BodyDic.erase(dicIndex);
        }

        b2Vec2 FB_GetPos(int dicIndex)
        {
            return BodyDic[dicIndex]->GetPosition();
        }

        void FB_SetPos(int dicIndex, b2Vec2 bodyPos)
        {
            BodyDic[dicIndex]->SetPosition(bodyPos);
        }

        void FB_GetAngle()
        {

        }

        void FB_SetAngle()
        {

        }

        void FB_GetVel()
        {

        }

        void FB_SetVel()
        {

        }

        void FB_GetAngularVel()
        {

        }


        void FB_SetAngularVel()
        {

        }

        bool FB_IsActive()
        {

        }

        void FB_SetActive()
        {

        }

        void FB_GetLinearDampaning()
        {

        }

        void FB_SetLinearDampaning()
        {

        }

        void FB_GetAngularDampaning()
        {

        }

        void FB_SetAngularDampaning()
        {

        }

        void FB_IsSleepAllowed()
        {

        }

        void FB_SetSleepAllowed()
        {

        }

        void FB_GetFixedRotation()
        {

        }

        void FB_SetFixedRotation()
        {

        }

        void FB_IsBullet()
        {

        }

        void FB_SetIsBullet()
        {

        }

        void FB_GetGravityScale()
        {

        }

        void FB_SetGravityScale()
        {

        }







        //------------Joints------------//

        //Prismatic Joints

        void PrJ_Create(int dicIndexA, int dicIndexB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor)
        { //Creates a Prismatic Joint
            PrismaticJoints* prismaticJoint = new PrismaticJoints;
            prismaticJoint->setJoint(BodyDic[dicIndexA]->GetBody(), BodyDic[dicIndexB]->GetBody(), worldAxis, lowerTranslation, upperTranslation, enableLimit, maxMotorForce, motorSpeed, enableMotor);
            jointDic[jointIncrement++] = prismaticJoint;
            std::cout << "Prismatic Joint Created" << std::endl;
        }

        PrismaticJoints* PrJ_GetJoint(int jointIndex) { //Gets a Prismatic Joint from the Joints dictionary
            return  dynamic_cast<PrismaticJoints*>(jointDic[jointIndex]->getJoint());
        }

        void PrJ_Destroy(int jointIndex) { //Destroys Prismatic Joints
            jointDic[jointIndex]->destroyJoint(jointDic[jointIndex]->getJoint());
            jointDic.erase(jointIndex);
            jointIncrement--;
        }

        b2Body* PrJ_GetBodyA(int jointIndex) { //Gets bodyA from a Prismatic Joint
            return jointDic[jointIndex]->getBodyA(jointDic[jointIndex]->getJoint());
        }

        void PrJ_SetBodyA(int jointIndex, int dicIndex) { //Sets bodyA for a Prismatic Joint
            jointDic[jointIndex]->setBodyA(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Body* PrJ_GetBodyB(int jointIndex) { //Gets bodyB from a Prismatic Joint
            return jointDic[jointIndex]->getBodyB(jointDic[jointIndex]->getJoint());
        }

        void PrJ_SetBodyB(int jointIndex, int dicIndex) { //Sets bodyB for a Prismatic Joint
            jointDic[jointIndex]->setBodyB(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Vec2 PrJ_GetAnchorA(int jointIndex) { //Gets anchorA from a Prismatic Joint
            return jointDic[jointIndex]->getAnchorA(jointDic[jointIndex]->getJoint());
        }

        void PrJ_SetAnchorA(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Prismatic Joint
            jointDic[jointIndex]->setAnchorA(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2Vec2 PrJ_GetAnchorB(int jointIndex) { //Gets anchorB from a Prismatic Joint
            return jointDic[jointIndex]->getAnchorB(jointDic[jointIndex]->getJoint());
        }

        void PrJ_SetAnchorB(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Prismatic Joint
            jointDic[jointIndex]->setAnchorB(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2JointUserData PrJ_GetUserData(int jointIndex) { //Gets the User Data from a Prismatic Joint
            return jointDic[jointIndex]->getUserData(jointDic[jointIndex]->getJoint());
        }

        bool PrJ_GetCollideConnected(int jointIndex) { //Checks if a joint has been collided with
            return jointDic[jointIndex]->getCollideConntected(jointDic[jointIndex]->getJoint());
        }

        b2Vec2 PrJ_GetReactionForce(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Force from a Prismatic Joint
            return jointDic[jointIndex]->getReactionForce(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        float PrJ_GetReactionTorque(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Torque from a Prismatic Joint
            return jointDic[jointIndex]->getReactionTorque(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        b2JointType PrJ_GetType(int jointIndex) { //Gets the type of Joints
            return jointDic[jointIndex]->getType(jointDic[jointIndex]->getJoint());
        }

        bool PrJ_IsEnabled(int jointIndex) { //Checks if the joint is enabled
            return jointDic[jointIndex]->isEnabled(jointDic[jointIndex]->getJoint());
        }

        void PrJ_ShiftOrigin(int jointIndex, b2Vec2 newOrigin) { //Shifts the origin for the joint
            jointDic[jointIndex]->shiftOrigin(jointDic[jointIndex]->getJoint(), newOrigin);
        }

        float PrJ_GetTranslation(int jointIndex) { //Gets the translation for a prismatic joint
            return  PrJ_GetJoint(jointIndex)->getJointTranslation();
        }

        void PrJ_SetTranslation(int jointIndex, float lowerTranslation, float upperTranslation) { //Sets the translation for a prismatic joint
            PrJ_GetJoint(jointIndex)->setJointTranslation(lowerTranslation, upperTranslation);
        }

        float PrJ_GetSpeed(int jointIndex) { //Gets the speed of the joint
            return PrJ_GetJoint(jointIndex)->getJointSpeed();
        }

        float PrJ_GetMotorForce(int jointIndex, float inv_dt) { //Gets the Motor Force for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getMotorForce(inv_dt);
        }

        float PrJ_GetMaxMotorForce(int jointIndex) { //Gets the Max Motor Force for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getMaxMotorForce();
        }

        void PrJ_SetMaxMotorForce(int jointIndex, float force) { //Sets the Max Motor Force for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setMaxMotorForce(force);
        }

        float PrJ_GetMotorSpeed(int jointIndex) { //Gets the Motor Speed for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getMotorSpeed();
        }

        void PrJ_SetMotorSpeed(int jointIndex, float speed) { //Sets the Motor Speed for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setMotorSpeed(speed);
        }

        b2Vec2 PrJ_GetWorldAxis(int jointIndex) { //Gets the World Axis from a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getWorldAxis();
        }

        void PrJ_SetWorldAxis(int jointIndex, b2Vec2 worldAxis) { //Sets the World Axis for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setWorldAxis(worldAxis);
        }

        float PrJ_GetLowerTranslation(int jointIndex) { //Gets the Lower Translation for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getLowerTranslation();
        }

        void PrJ_SetLowerTranslation(int jointIndex, float lowerTranslation) { //Sets the Upper Translation for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setLowerTranslation(lowerTranslation);
        }

        float PrJ_GetUpperTranslation(int jointIndex) { //Gets the Upper Translation for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getUpperTranslation();
        }

        void PrJ_SetUpperTranslation(int jointIndex, float upperTranslation) { //Sets the Upper Translation for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setUpperTranslation(upperTranslation);
        }

        bool PrJ_GetEnableLimit(int jointIndex) { //Gets if limit has been enabled for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getEnableLimit();
        }

        void PrJ_SetEnableLimit(int jointIndex, bool isEnabled) { //Sets if the limit is enabled for a Prismatic Joint 
            PrJ_GetJoint(jointIndex)->setEnableLimit(isEnabled);
        }

        bool PrJ_GetEnableMotor(int jointIndex) { //Gets if motor has been enabled for a Prismatic Joint
            return PrJ_GetJoint(jointIndex)->getEnableMotor();
        }

        void PrJ_SetEnableMotor(int jointIndex, bool isEnabled) { //Sets if the motor is enabled for a Prismatic Joint
            PrJ_GetJoint(jointIndex)->setEnableMotor(isEnabled);
        }

        //Weld Joints

        void WJ_Create(int dicIndexA, int dicIndexB) {  //Creates a Weld Joint
            WeldJoints* weldJoint = new WeldJoints;
            weldJoint->setJoint(BodyDic[dicIndexA]->GetBody(), BodyDic[dicIndexB]->GetBody());
            jointDic[jointIncrement++] = weldJoint;
            std::cout << "Weld Joint Created" << std::endl;
        }

        WeldJoints* WJ_GetJoint(int jointIndex) { //Gets a Weld Joint from the Joints dictionary
            return  dynamic_cast<WeldJoints*>(jointDic[jointIndex]->getJoint());
        }

        void WJ_Destroy(int jointIndex) { //Destorys Weld Joints
            jointDic[jointIndex]->destroyJoint(jointDic[jointIndex]->getJoint());
            jointDic.erase(jointIndex);
            jointIncrement--;
        }

        b2Body* WJ_GetBodyA(int jointIndex) { //Gets bodyA from a Weld Joint
            return jointDic[jointIndex]->getBodyA(jointDic[jointIndex]->getJoint());
        }

        void WJ_SetBodyA(int jointIndex, int dicIndex) { //Sets bodyA for a Weld Joint
            jointDic[jointIndex]->setBodyA(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Body* WJ_GetBodyB(int jointIndex) { //Gets bodyB for a Weld Joint
            return jointDic[jointIndex]->getBodyB(jointDic[jointIndex]->getJoint());
        }

        void WJ_SetBodyB(int jointIndex, int dicIndex) { //Sets bodyB for a Weld Joint
            jointDic[jointIndex]->setBodyB(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Vec2 WJ_GetAnchorA(int jointIndex) { //Gets anchorA for a Weld Joint
            return jointDic[jointIndex]->getAnchorA(jointDic[jointIndex]->getJoint());
        }

        void WJ_SetAnchorA(int jointIndex, b2Vec2 newAnchor) { //Set anchorA for a Weld Joint
            jointDic[jointIndex]->setAnchorA(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2Vec2 WJ_GetAnchorB(int jointIndex) { //Gets anchorB for a Weld Joint
            return jointDic[jointIndex]->getAnchorB(jointDic[jointIndex]->getJoint());
        }

        void WJ_SetAnchorB(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Weld Joint
            jointDic[jointIndex]->setAnchorB(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2JointUserData WJ_GetUserData(int jointIndex) { //Gets the User Data for a Weld Joint
            return jointDic[jointIndex]->getUserData(jointDic[jointIndex]->getJoint());
        }

        bool WJ_GetCollideConnected(int jointIndex) { //Checks if a joint has been collided with
            return jointDic[jointIndex]->getCollideConntected(jointDic[jointIndex]->getJoint());
        }

        b2Vec2 WJ_GetReactionForce(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Force for a Weld Joint
            return jointDic[jointIndex]->getReactionForce(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        float WJ_GetReactionTorque(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Torque for a Weld Joint
            return jointDic[jointIndex]->getReactionTorque(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        b2JointType WJ_GetType(int jointIndex) { //Gets the type of Joint
            return jointDic[jointIndex]->getType(jointDic[jointIndex]->getJoint());
        }

        bool WJ_IsEnabled(int jointIndex) {//Checks if a Weld Joint is enabled
            return jointDic[jointIndex]->isEnabled(jointDic[jointIndex]->getJoint());
        }

        void WJ_ShiftOrigin(int jointIndex, b2Vec2 newOrigin) { //Shifts the Origin for a Weld Joint
            jointDic[jointIndex]->shiftOrigin(jointDic[jointIndex]->getJoint(), newOrigin);
        }

        void WJ_SetDamping(int jointIndex, float damping) { //Sets the Damping for a Weld Joint
            WJ_GetJoint(jointIndex)->setDamping(damping);
        }

        float WJ_GetDamping(int jointIndex) { //Gets the Damping for a Weld Joint
            return WJ_GetJoint(jointIndex)->getDamping();
        }

        void WJ_SetStiffness(int jointIndex, float stiffness) { //Sets the Stiffness for a Weld Joint
            WJ_GetJoint(jointIndex)->setStiffness(stiffness);
        }

        float WJ_GetStiffness(int jointIndex) { //Gets the Stiffness for a Weld Joint
            return WJ_GetJoint(jointIndex)->getStiffness();
        }

        //Pulley Joints

        void PJ_Create(int dicIndexA, int dicIndexB, b2Vec2 position1, b2Vec2 position2, float ratio, float lengthA, float lengthB) { //Creates a Pulley Joint
            PulleyJoints* pulleyJoint = new PulleyJoints;
            pulleyJoint->setJoint(BodyDic[dicIndexA]->GetBody(), BodyDic[dicIndexB]->GetBody(), position1, position2, ratio, lengthA, lengthB);
            jointDic[jointIncrement++] = pulleyJoint;
            std::cout << "Pulley Joint Created" << std::endl;
        }

        PulleyJoints* PJ_GetJoint(int jointIndex) {//Gets a Pulley Joint
            return  dynamic_cast<PulleyJoints*>(jointDic[jointIndex]->getJoint());
        }

        void PJ_Destroy(int jointIndex) {//Destroys a Pulley Joint
            jointDic[jointIndex]->destroyJoint(jointDic[jointIndex]->getJoint());
            jointDic.erase(jointIndex);
            jointIncrement--;
        }

        b2Body* PJ_GetBodyA(int jointIndex) { //Gets bodyA from a Pulley Joint
            return jointDic[jointIndex]->getBodyA(jointDic[jointIndex]->getJoint());
        }

        void PJ_SetBodyA(int jointIndex, int dicIndex) { //Sets bodyA for a Pulley Joint
            jointDic[jointIndex]->setBodyA(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Body* PJ_GetBodyB(int jointIndex) { //Gets bodyB from a Pulley Joint
            return jointDic[jointIndex]->getBodyB(jointDic[jointIndex]->getJoint());
        }

        void PJ_SetBodyB(int jointIndex, int dicIndex) { //Sets bodyB for a Pulley Joint
            jointDic[jointIndex]->setBodyB(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Vec2 PJ_GetAnchorA(int jointIndex) { //Gets anchorA from a Pulley Joint
            return jointDic[jointIndex]->getAnchorA(jointDic[jointIndex]->getJoint());
        }

        void PJ_SetAnchorA(int jointIndex, b2Vec2 newAnchor) { //Sets anchorA for a Pulley Joint
            jointDic[jointIndex]->setAnchorA(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2Vec2 PJ_GetAnchorB(int jointIndex) { //Gets anchorB from a Pulley Joint
            return jointDic[jointIndex]->getAnchorB(jointDic[jointIndex]->getJoint());
        }

        void PJ_SetAnchorB(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Pulley Joint
            jointDic[jointIndex]->setAnchorB(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2JointUserData PJ_GetUserData(int jointIndex) { //Gets the User Data from a Pulley Joint
            return jointDic[jointIndex]->getUserData(jointDic[jointIndex]->getJoint());
        }

        bool PJ_GetCollideConnected(int jointIndex) { //Checks if a joint has been collided with
            return jointDic[jointIndex]->getCollideConntected(jointDic[jointIndex]->getJoint());
        }

        b2Vec2 PJ_GetReactionForce(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Force for a Pulley Joint
            return jointDic[jointIndex]->getReactionForce(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        float PJ_GetReactionTorque(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Torque for a Pulley Joint
            return jointDic[jointIndex]->getReactionTorque(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        b2JointType PJ_GetType(int jointIndex) { //Get the type of joint
            return jointDic[jointIndex]->getType(jointDic[jointIndex]->getJoint());
        }

        bool PJ_IsEnabled(int jointIndex) { //Checks if a Pulley Joint is enabled
            return jointDic[jointIndex]->isEnabled(jointDic[jointIndex]->getJoint());
        }

        void PJ_ShiftOrigin(int jointIndex, b2Vec2 newOrigin) { //Shifts the origin for a Pulley Joint
            jointDic[jointIndex]->shiftOrigin(jointDic[jointIndex]->getJoint(), newOrigin);
        }

        float PJ_GetLengthA(int jointIndex) { //Gets lengthA from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getLengthA();
        }

        void PJ_SetLengthA(int jointIndex, float lengthA) { //Sets lengthA for a Pulley Joint
            PJ_GetJoint(jointIndex)->setLengthA(lengthA);
        }

        float PJ_GetLengthB(int jointIndex) { //Gets lengthB from Pulley Joint
            return PJ_GetJoint(jointIndex)->getLengthB();
        }

        void PJ_SetLengthB(int jointIndex, float lengthB) { //Sets LengthB for a Pulley Joint
            PJ_GetJoint(jointIndex)->setLengthB(lengthB);
        }

        float PJ_GetCurrentLengthA(int jointIndex) { //Gets the Current Length A from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getCurrentLengthA();
        }

        float PJ_GetCurrentLengthB(int jointIndex) { //Gets the Current Length B from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getCurrentLengthB();
        }

        float PJ_GetRatio(int jointIndex) { //Gets ratio from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getRatio();
        }

        void PJ_SetRatio(int jointIndex, float ratio) { //Sets ratio for a Pulley Joint
            PJ_GetJoint(jointIndex)->setRatio(ratio);
        }

        b2Vec2 PJ_GetGroundAnchorA(int jointIndex) { //Gets Ground Anchor A from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getGroundAnchorA();
        }

        b2Vec2 PJ_GetGroundAnchorB(int jointIndex) { //Gets Ground Anchor B from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getGroundAnchorB();
        }

        b2Vec2 PJ_GetPosition1(int jointIndex) { //Gets position1 from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getPosition1();
        }

        void PJ_SetPosition1(int jointIndex, b2Vec2 position) { //Sets position1 for a Pulley Joint
            PJ_GetJoint(jointIndex)->setPosition1(position);
        }

        b2Vec2 PJ_GetPosition2(int jointIndex) { //Gets position2 from a Pulley Joint
            return PJ_GetJoint(jointIndex)->getPosition2();
        }

        void PJ_SetPosition2(int jointIndex, b2Vec2 position) { //Sets position2 for Pulley Joints
            PJ_GetJoint(jointIndex)->setPosition2(position);
        }
        //Revolute Joints

        void RJ_Create(int dicIndexA, int dicIndexB, bool enableLimit, float lowerAngle, float upperAngle, bool enableMotor, float motorSpeed, float maxMotorTorque) { //Creates a Revolute Joint
            RevoluteJoints* revoluteJoint = new RevoluteJoints;
            revoluteJoint->setJoint(BodyDic[dicIndexA]->GetBody(), BodyDic[dicIndexB]->GetBody(), enableLimit, lowerAngle, upperAngle, enableMotor, motorSpeed, maxMotorTorque);
            jointDic[jointIncrement++] = revoluteJoint;
            std::cout << "Revolute Joint Created" << std::endl;
        }

        RevoluteJoints* RJ_GetJoint(int jointIndex) { //Gets a Revolute Joint
            return  dynamic_cast<RevoluteJoints*>(jointDic[jointIndex]->getJoint());
        }

        void RJ_Destroy(int jointIndex) { //Destroys a Revolute Joint
            jointDic[jointIndex]->destroyJoint(jointDic[jointIndex]->getJoint());
            jointDic.erase(jointIndex);
            jointIncrement--;
        }

        b2Body* RJ_GetBodyA(int jointIndex) { //Gets bodyA from a Revolute Joint
            return jointDic[jointIndex]->getBodyA(jointDic[jointIndex]->getJoint());
        }

        void RJ_SetBodyA(int jointIndex, int dicIndex) { //Sets bodyA for a Revolute Joint
            jointDic[jointIndex]->setBodyA(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Body* RJ_GetBodyB(int jointIndex) { //Gets bodyB from a Revolute Joint
            return jointDic[jointIndex]->getBodyB(jointDic[jointIndex]->getJoint());
        }

        void RJ_SetBodyB(int jointIndex, int dicIndex) { //Sets bodyB for a Revolute Joint
            jointDic[jointIndex]->setBodyB(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Vec2 RJ_GetAnchorA(int jointIndex) { //Gets anchorA from a Revolute Joint
            return jointDic[jointIndex]->getAnchorA(jointDic[jointIndex]->getJoint());
        }

        void RJ_SetAnchorA(int jointIndex, b2Vec2 newAnchor) { //Sets anchorA for a Revolute Joint
            jointDic[jointIndex]->setAnchorA(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2Vec2 RJ_GetAnchorB(int jointIndex) { //Gets anchorB from a Revolute Joint
            return jointDic[jointIndex]->getAnchorB(jointDic[jointIndex]->getJoint());
        }

        void RJ_SetAnchorB(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Revolute Joint
            jointDic[jointIndex]->setAnchorB(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2JointUserData RJ_GetUserData(int jointIndex) { //Gets User Data from a Revolute Joint
            return jointDic[jointIndex]->getUserData(jointDic[jointIndex]->getJoint());
        }

        bool RJ_GetCollideConnected(int jointIndex) { //Checks if a joint has been collided with
            return jointDic[jointIndex]->getCollideConntected(jointDic[jointIndex]->getJoint());
        }

        b2Vec2 RJ_GetReactionForce(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Force from a Revolute Joint
            return jointDic[jointIndex]->getReactionForce(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        float RJ_GetReactionTorque(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Torque from a Revolute Joint
            return jointDic[jointIndex]->getReactionTorque(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        b2JointType RJ_GetType(int jointIndex) { //Gets the type of Joint
            return jointDic[jointIndex]->getType(jointDic[jointIndex]->getJoint());
        }

        bool RJ_IsEnabled(int jointIndex) { //Checks if the Joint is enabled
            return jointDic[jointIndex]->isEnabled(jointDic[jointIndex]->getJoint());
        }

        void RJ_ShiftOrigin(int jointIndex, b2Vec2 newOrigin) { //Shifts the origin of a Revolute Joint
            jointDic[jointIndex]->shiftOrigin(jointDic[jointIndex]->getJoint(), newOrigin);
        }

        float RJ_GetAngle(int jointIndex) { //Gets the Angle from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getJointAngle();
        }

        float RJ_GetSpeed(int jointIndex) { //Gets the Joint Speed from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getJointSpeed();
        }

        float RJ_GetMotorSpeed(int jointIndex) { //Gets the Motor Speed from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getMotorSpeed();
        }

        void RJ_SetMotorSpeed(int jointIndex, float speed) { //Sets the Motor Speed for a Revolute Joint
            RJ_GetJoint(jointIndex)->setMotorSpeed(speed);
        }

        float RJ_GetMotorTorque(int jointIndex, float inv_dt) { //Gets the Motor Torque from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getMotorTorque(inv_dt);
        }

        void RJ_SetMaxMotorTorque(int jointIndex, float torque) { //Sets the Max Motor Torque for Revolute Joint
            RJ_GetJoint(jointIndex)->setMaxMotorTorque(torque);
        }

        bool RJ_GetEnableLimit(int jointIndex) { //Checks if the limit for a Revolute Joint is enabled
            return RJ_GetJoint(jointIndex)->getEnableLimit();
        }

        void RJ_SetEnableLimit(int jointIndex, bool isEnabled) { //Sets if the limit for a Revolute Joint is enabled
            RJ_GetJoint(jointIndex)->setEnableLimit(isEnabled);
        }

        bool RJ_GetEnableMotor(int jointIndex) { //Checks if the motor for a Revolute Joint is enabled
            return RJ_GetJoint(jointIndex)->getEnableMotor();
        }

        void RJ_SetEnableMotor(int jointIndex, bool isEnabled) { //Sets if the motor for a Revolute Joint is enabled
            RJ_GetJoint(jointIndex)->setEnableMotor(isEnabled);
        }

        float RJ_GetLowerAngle(int jointIndex) { //Gets the Lower Angle from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getLowerAngle();
        }

        void RJ_SetLowerAngle(int jointIndex, float lowerAngle) { //Sets the Lower Angle for a Revolute Joint
            RJ_GetJoint(jointIndex)->setLowerAngle(lowerAngle);
        }

        float RJ_GetUpperAngle(int jointIndex) { //Gets the Upper Angle from a Revolute Joint
            return RJ_GetJoint(jointIndex)->getUpperAngle();
        }

        void RJ_SetUpperAngle(int jointIndex, float upperAngle) { //Sets the Upper Angle for a Revolute Joint
            RJ_GetJoint(jointIndex)->setUpperAngle(upperAngle);
        }

        //Distance Joints

        void DJ_Create(int dicIndexA, int dicIndexB, b2Vec2 anchorA, b2Vec2 anchorB, float length) { //Creates a Distance Joint
            DistanceJoints* distanceJoint = new DistanceJoints;
            distanceJoint->setJoint(BodyDic[dicIndexA]->GetBody(), BodyDic[dicIndexB]->GetBody(), anchorA, anchorB, length);
            jointDic[jointIncrement++] = distanceJoint;
            std::cout << "Distance Joint Created" << std::endl;
        }

        DistanceJoints* DJ_GetJoint(int jointIndex) { //Gets a Distance Joint
            return  dynamic_cast<DistanceJoints*>(jointDic[jointIndex]->getJoint());
        }

        void DJ_Destroy(int jointIndex) { //Destroys a Distance Joint
            jointDic[jointIndex]->destroyJoint(jointDic[jointIndex]->getJoint());
            jointDic.erase(jointIndex);
            jointIncrement--;
        }

        b2Body* DJ_GetBodyA(int jointIndex) { //Gets bodyA from a Distance Joint
            return jointDic[jointIndex]->getBodyA(jointDic[jointIndex]->getJoint());
        }

        void DJ_SetBodyA(int jointIndex, int dicIndex) { //Sets bodyA for a Distance Joint
            jointDic[jointIndex]->setBodyA(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Body* DJ_GetBodyB(int jointIndex) { //Gets bodyB from a Distance Joint
            return jointDic[jointIndex]->getBodyB(jointDic[jointIndex]->getJoint());
        }

        void DJ_SetBodyB(int jointIndex, int dicIndex) { //Sets bodyB for a Distance Joint
            jointDic[jointIndex]->setBodyB(jointDic[jointIndex]->getJoint(), BodyDic[dicIndex]->GetBody());
        }

        b2Vec2 DJ_GetAnchorA(int jointIndex) { //Gets anchorA from a Distance Joint
            return jointDic[jointIndex]->getAnchorA(jointDic[jointIndex]->getJoint());
        }

        void DJ_SetAnchorA(int jointIndex, b2Vec2 newAnchor) { //Sets anchorA for a Distance Joint
            jointDic[jointIndex]->setAnchorA(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2Vec2 DJ_GetAnchorB(int jointIndex) { //Gets anchorB for a Distance Joint
            return jointDic[jointIndex]->getAnchorB(jointDic[jointIndex]->getJoint());
        }

        void DJ_SetAnchorB(int jointIndex, b2Vec2 newAnchor) { //Sets anchorB for a Distance Joint
            jointDic[jointIndex]->setAnchorB(jointDic[jointIndex]->getJoint(), newAnchor);
        }

        b2JointUserData DJ_GetUserData(int jointIndex) { //Gets the User Data from a Distance Joint
            return jointDic[jointIndex]->getUserData(jointDic[jointIndex]->getJoint());
        }

        bool DJ_GetCollideConnected(int jointIndex) { //Checks if a joint has been collided with
            return jointDic[jointIndex]->getCollideConntected(jointDic[jointIndex]->getJoint());
        }

        b2Vec2 DJ_GetReactionForce(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Force from a Distance Joint
            return jointDic[jointIndex]->getReactionForce(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        float DJ_GetReactionTorque(int jointIndex, float inverseDeltaTime) { //Gets the Reaction Torque from a Distance Joint
            return jointDic[jointIndex]->getReactionTorque(jointDic[jointIndex]->getJoint(), inverseDeltaTime);
        }

        b2JointType DJ_GetType(int jointIndex) { //Gets the type of Joint
            return jointDic[jointIndex]->getType(jointDic[jointIndex]->getJoint());
        }

        bool DJ_IsEnabled(int jointIndex) { //Checks if the Distance Joint is enabled
            return jointDic[jointIndex]->isEnabled(jointDic[jointIndex]->getJoint());
        }

        void DJ_ShiftOrigin(int jointIndex, b2Vec2 newOrigin) { //Shifts the origin of a Distance Joint
            jointDic[jointIndex]->shiftOrigin(jointDic[jointIndex]->getJoint(), newOrigin);
        }

        float DJ_GetLength(int jointIndex) { //Gets the length of a Distance Joint
          return DJ_GetJoint(jointIndex)->getLength();
        }

        void DJ_SetLength(int jointIndex, float length) { //Sets the length of a Distance Joint
            DJ_GetJoint(jointIndex)->setLength(length);
        }
    
    };
}