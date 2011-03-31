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
#include "CollisionObject.h"
#include "Sound.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define JUMP_SAMPLE "resources/sound/boing.wav"

class PlayerUpdater : public osg::NodeCallback 
{
public:
    PlayerUpdater();
    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
    osg::Vec3 calculateNextPosition();
};
