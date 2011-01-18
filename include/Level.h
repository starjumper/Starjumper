#pragma once

#include <string>
#include <stdexcept>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"

#define DEFAULT_COLOR	osg::Vec4(0.8, 0.5, 0.7, 1.0)

class Level
{
private:
    osg::PositionAttitudeTransform *_level;
    
    btDynamicsWorld *_world;
    btCompoundShape *_collisionShapes;
  
public:
    Level(btDynamicsWorld *world, const std::string mapfile);
    
    void loadMapFromFile(const std::string mapfile);
    osg::Vec3 getFromVector(const rapidxml::xml_node<> &node) const;
    osg::Vec3 getToVector(const rapidxml::xml_node<> &node) const;
    void addCuboid(const rapidxml::xml_node<> &cuboidNode);
    void addTunnel(const rapidxml::xml_node<> &tunnelNode);
    
public:
    Level(btDynamicsWorld *world, const std::string mapfile);
};