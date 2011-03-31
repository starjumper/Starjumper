#pragma once

#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include "CollisionObject.h"

#define ACCELERATION_CUBOID_TEXTURE "resources/textures/acceleration.png"
#define DECELERATION_CUBOID_TEXTURE "resources/textures/deceleration.png"

class Cuboid : public CollisionObject
{
private:
    osg::Vec3 _from;
    osg::Vec3 _size;
    
protected:
    osg::Geometry *_drawable;
        
public:  
    Cuboid(const osg::Vec3 &from, const osg::Vec3 &size);
    
    btRigidBody *getRigidBody();
    virtual void collide() = 0;
};

class DefaultCuboid : public Cuboid
{
    public:
        DefaultCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        void collide() { }
};

class TexturedCuboid : public Cuboid
{
    private:
        osg::Texture2D *_texture;
        
    protected:
        osg::Geometry *_textureDrawable;
    
    public:
        TexturedCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        void setTexture(osg::Image *image);
        void collide() = 0;
};

class AccelerationCuboid : public TexturedCuboid
{
    public:
        AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        void collide();        
};

class DecelerationCuboid : public TexturedCuboid
{
    public:
        DecelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        void collide();        
};