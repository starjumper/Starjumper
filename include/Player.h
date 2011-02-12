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
#include "PlayerState.h"

// global Player settings
#define PLAYER_MODEL        "resources/models/player.osg"
#define PLAYER_SCALE        osg::Vec3(0.2, 0.2, 0.2)
#define PLAYER_ATTITUDE     osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,5.0))
#define PLAYER_BBOX_EXTENTS btVector3(1, 1, 1)

class Player
{
private:
    osg::PositionAttitudeTransform *_player;
    osg::PositionAttitudeTransform *patPlayer;
    
    btPairCachingGhostObject *_playerGhostObject;
    btKinematicCharacterController *_playerController;
    PlayerState *_playerState;
    
    void initializePlayerModel();
    void initializePlayerPhysics();
    
public:
    Player();
    ~Player();
    
    // getter methods
    osg::PositionAttitudeTransform *getNode();
    btPairCachingGhostObject *getGhostObject();
    btKinematicCharacterController *getController();
    PlayerState *getPlayerState();
    
    void setPosition(osg::Vec3 position);
};