#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#define TUNNEL_MODEL_FILE		"models/tunnel.osg"
#define DEFAULT_COLOR	        osg::Vec4f(0.8f, 0.5f, 0.7f, 1.0f)

class Level
{
private:
    osg::PositionAttitudeTransform *_level;
    
    btDynamicsWorld *_world;
    btCompoundShape *_collisionShapes;
  
public:
    Level(/*btDynamicsWorld *world, */const std::string mapfile);
    
    void loadMapFromFile(const std::string mapfile);
    osg::Vec3 getFromVector(const rapidxml::xml_node<> &node) const;
    osg::Vec3 getToVector(const rapidxml::xml_node<> &node) const;
    void addCuboid(const rapidxml::xml_node<> &cuboidNode);
    void addTunnel(const rapidxml::xml_node<> &tunnelNode);
    
    osg::PositionAttitudeTransform *getNode();
    btCompoundShape *getCollisionShape();
};