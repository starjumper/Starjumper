#pragma once

#include <osg/Group>
#include <osgViewer/Viewer>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "Player.h"
#include "LazyCameraManipulator.h"
#include "Cuboid.h"

#define PHYSICS_WORLD_MIN           -1000, -1000, -1000
#define PHYSICS_WORLD_MAX           1000, 1000, 1000
#define PHYSICS_WORLD_GRAVITY       btVector3(0.0, 0.0, -40.0)

#define LEVEL_CAMERA_HOME_EYE       osg::Vec3(0.0, 1.0, 2.0)
#define LEVEL_CAMERA_HOME_CENTER    osg::Vec3(0.0, 10.0, 0.0)
#define LEVEL_CAMERA_HOME_UP        osg::Vec3(0.0, -10.0, 5.0)

class Level : public osg::Group
{
private:    
    btDynamicsWorld *_physicsWorld;
	
    void loadMapFromFile(const std::string &mapfile);
    void initializePhysicsWorld();
    
    osg::Vec3 getVectorFromXMLNode(const std::string &name, const rapidxml::xml_node<> &node) const;
            
public:
    Level(const std::string &mapfile);    
    
    btDynamicsWorld *getPhysicsWorld() { return _physicsWorld; };    
};

class LevelUpdater : public osg::NodeCallback
{
private:
    Level *_level;
    float _previousStepTime;
    
public:
    LevelUpdater(Level *level);
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};