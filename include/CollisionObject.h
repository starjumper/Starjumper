#pragma once

#include <osg/Geode>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>


class CollisionObject : public osg::Geode
{    
public:
    CollisionObject();
    
    virtual void collide() = 0;
    virtual btRigidBody *getRigidBody() = 0;
    
};