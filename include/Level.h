#pragma once

#include <string>
#include <stdexcept>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>

#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"

#define DEFAULT_COLOR osg::Vec4(0.8, 0.5, 0.7, 1.0)

class Level
{
private:
    osg::PositionAttitudeTransform *_level;
    
public:
    Level(std::string mapfile);
    void loadMapFromFile(std::string mapfile);

    osg::Vec3 getFromVector(rapidxml::xml_node<> &node);
    osg::Vec3 getToVector(rapidxml::xml_node<> &node);
    
    void addCuboid(rapidxml::xml_node<> &cuboidNode);
    void addTunnel(rapidxml::xml_node<> &tunnelNode);
};