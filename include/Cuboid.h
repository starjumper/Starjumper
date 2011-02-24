#pragma once

#include <iostream>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/Texture2D>
#include <osg/Material>

#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "types.h"

#define DEFAULT_CUBOID_COLOR        osg::Vec4f(0.8f, 0.5f, 0.7f, 0.5f)
#define ACCELERATION_CUBOID_TEXTURE "resources/textures/acceleration.png"
#define DECELERATION_CUBOID_TEXTURE "resources/textures/deceleration.png"

class Cuboid : public osg::Referenced
{
    private:
    	osg::Geode *_node;
    	osg::Drawable *_drawable;
    	
        btRigidBody *_rigidBody;
        void constructRigidBody(const osg::Vec3 &center, const float width, const float depth, const float height);

    protected:
        osg::Texture2D *_texture;        
                
    public:  
        Cuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        
        osg::Node *getNode();
        osg::Drawable *getDrawable();
        btRigidBody *getRigidBody();
};

class AccelerationCuboid : public Cuboid
{
    public:
        AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
};

class DecelerationCuboid : public Cuboid
{
    public:
        DecelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
};