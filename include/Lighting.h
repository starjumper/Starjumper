#pragma once

#include <osg/Group>
#include <osg/Node>

#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateAttribute>
#include <osg/Geometry>
#include <osg/Point>

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

class Lighting
{
private:
    osg::Light *_light;
    
public:
    Lighting();
    
    // getter methods
	osg::Node *getLights(osg::StateSet *rootStateSet);
	void setLightPosition(osg::Vec3 position);
};