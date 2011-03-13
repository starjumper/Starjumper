#pragma once

#include <stdexcept>

#include <osg/Group>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateAttribute>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "KinematicCharacterController.h"
#include "types.h"
#include "PlayerState.h"
#include "ParticleEffectFactory.h"

// global Player settings
#define PLAYER_MODEL        "resources/models/player.osg"
#define PLAYER_MODEL_HIGH	"resources/models/player_high.osg"
#define PLAYER_SCALE        osg::Vec3(0.2, 0.2, 0.2)
#define PLAYER_ATTITUDE     osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,1.0))
#define PLAYER_BBOX_EXTENTS btVector3(0.5, 0.5, 0.5)

class Player : public osg::Group
{
private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _playerPAT;
    
    ParticleEffect *_mainEngine;
    ParticleEffect *_leftEngine;
    ParticleEffect *_rightEngine;
    osg::ref_ptr<osg::Group> _particleEffects;
    
    btPairCachingGhostObject *_playerGhostObject;
    KinematicCharacterController *_playerController;
    PlayerState *_playerState;
	
    const std::vector<float> *_deadlyAltitudes;
    
    void initializePlayerModel();
    void initializePlayerPhysics();
    void initializePlayerEffects();
    
public:
    Player();
    ~Player();
    
    // getter methods
    osg::PositionAttitudeTransform *getPlayerPAT() const;
    btPairCachingGhostObject *getGhostObject() const;
    KinematicCharacterController *getController() const;
    PlayerState *getPlayerState() const;
    
    void setPosition(const osg::Vec3 position);
    void setAngles(const float angleX = 0.0f, const float angleY = 0.0f, const float angleZ = 180.0f);
    
    void toHomePosition();
    
    void setDeadlyAltitudes(const std::vector<float> *values);
    const std::vector<float> *getDeadlyAltitudes();
    
    void setEnginesAccelerating(const float speed);
    void setEnginesDecelerating(const float speed);
};
