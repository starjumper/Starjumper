#pragma once

#include <stdexcept>

#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "KinematicCharacterController.h"
#include "PlayerState.h"

#define PLAYER_MODEL            "resources/models/player.osg"
#define PLAYER_SCALE            osg::Vec3(0.2, 0.2, 0.2)
#define PLAYER_ATTITUDE         osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,1.0))
#define PLAYER_HOME_POSITION    osg::Vec3(0.0, 10.0, 5.0)
#define PLAYER_BBOX_EXTENTS     btVector3(0.5, 0.5, 0.5)

class Player : public osg::PositionAttitudeTransform
{
private:
    PlayerState *_playerState;
    
    static Player *_instance;
    
    btPairCachingGhostObject *_playerGhostObject;
    KinematicCharacterController *_playerController;

    Player();
    
    void loadPlayerModel();
    void initializePhysics();
    void resetPosition();
        
public:
    static Player *getInstance();
    
    KinematicCharacterController *getController() const { return _playerController; }
    btPairCachingGhostObject *getGhostObject() const { return _playerGhostObject; }  
    PlayerState *getPlayerState() const { return _playerState; }
    
    void setAngles(const float angleX = 0.0f, const float angleY = 0.0f, const float angleZ = 180.0f);
};
