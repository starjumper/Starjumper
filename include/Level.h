#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <osg/Group>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "Cuboid.h"
#include "Tunnel.h"

#define DEFAULT_COLOR	        osg::Vec4f(0.0f, 0.5f, 0.7f, 1.0f)

class Level : public osg::Group
{
private:
    std::vector<btRigidBody *> _collisionObjects;

    std::vector<float> _minZValues;

protected:
    osg::Vec3 getVectorFromXMLNode(const std::string &name, const rapidxml::xml_node<> &node) const;
    void addCuboid(const rapidxml::xml_node<> &cuboidNode);
    void addTunnel(const rapidxml::xml_node<> &tunnelNode);
  
public:
    Level(const std::string &mapfile);
    
    void loadMapFromFile(const std::string &mapfile);
    
    std::vector<btRigidBody *> getCollisionObjects() const;
    
    const std::vector<float> *getMinZValues() const;
};