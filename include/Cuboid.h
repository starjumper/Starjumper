#pragma once

#include <osg/Drawable>
#include <osg/Geometry>

#include "CollisionObject.h"

class Cuboid : public CollisionObject
{
private:
    osg::Vec3 _from;
    osg::Vec3 _size;
        
public:  
    Cuboid(const osg::Vec3 &from, const osg::Vec3 &size);
    
    btRigidBody *getRigidBody();
    void collide();
};