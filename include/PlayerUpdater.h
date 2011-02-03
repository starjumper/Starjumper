#pragma once

#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osgbBullet/Utils.h>

#include "Player.h"

class PlayerUpdater : public osg::NodeCallback 
{
private:
    Player *_player;
    
public:
    PlayerUpdater(Player *player);
    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
    
    osg::Vec3 updatePhysics(PlayerState *playerState);
};