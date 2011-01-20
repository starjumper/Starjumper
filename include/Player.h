#pragma once

#include <stdexcept>

#include <osg/Group>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#define PLAYER_MODEL        "resources/models/player.osg"
#define PLAYER_SCALE        osg::Vec3(1.0, 1.0, 1.0)
#define PLAYER_ATTITUDE     osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,1.0))
#define PLAYER_BBOX_EXTENTS btVector3(1, 1, 1)

class Player
{
private:
    osg::PositionAttitudeTransform *_player;
    
    btDynamicsWorld *_world;
    btGhostObject *_playerGhostObject;
    
    void initializePlayerModel();
    void initializePlayerPhysics();
    
public:
    Player();
    Player(btDynamicsWorld *world);
    osg::PositionAttitudeTransform *getNode();
    btGhostObject *getPlayerGhostObject();
};