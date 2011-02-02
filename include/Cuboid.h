#pragma once

#include <iostream>

#include <osg/Node>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

#define DEFAULT_CUBOID_COLOR        osg::Vec4f(0.8f, 0.5f, 0.7f, 1.0f)
#define ACCELERATION_CUBOID_COLOR   osg::Vec4f(0.0f, 1.0f, 1.0f, 0.1f)
#define ACCELERATION_CUBOID_TEXTURE "resources/textures/example.png"

class Cuboid : public osg::Referenced
{
    private:
    	osg::Geode *_node;
    	osg::ShapeDrawable *_shapeDrawable;
                
    public:  
        Cuboid(const osg::Vec3 &from, const osg::Vec3 &size);
        
        osg::Node *getNode();
        osg::ShapeDrawable *getShapeDrawable();
};

class AccelerationCuboid : public Cuboid
{
    public:
        AccelerationCuboid(const osg::Vec3 &from, const osg::Vec3 &size);
};