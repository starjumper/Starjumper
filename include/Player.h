#pragma once

#include <stdexcept>

#include <osg/Group>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "types.h"

#define PLAYER_MODEL        "resources/models/player.osg"
#define PLAYER_SCALE        osg::Vec3(0.2, 0.2, 0.2)
#define PLAYER_ATTITUDE     osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,5.0))
#define PLAYER_BBOX_EXTENTS btVector3(1, 1, 1)

class Player
{
private:
    osg::PositionAttitudeTransform *_player;
    
    btPairCachingGhostObject *_playerGhostObject;
    btKinematicCharacterController *_playerController;
    
    void initializePlayerModel();
    void initializePlayerPhysics();
    
public:
    Player();
    
    // player control
    void moveLeft(bool &keyState);
    void moveRight(bool &keyState);
    void accelerate(bool &keyState);
    void decelerate(bool &keyState);
    void jump(bool &keyState);
    
    // getter methods
    osg::PositionAttitudeTransform *getNode();
    btCollisionObject *getCollisionObject();
    btKinematicCharacterController *getController();
};