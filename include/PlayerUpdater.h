#pragma once

#include <iostream>
#include <algorithm>

#include <osg/Group>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <osgbBullet/Utils.h>

#include "Player.h"
#include "Cuboid.h"

class PlayerUpdater : public osg::NodeCallback 
{
private:
    Player *_player;
    
public:
    PlayerUpdater(Player *player);
    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
    
    osg::Vec3 calculateNextPosition(PlayerState *playerState);
};