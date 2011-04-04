#pragma once

#include <osg/Group>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osgViewer/Viewer>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_iterators.hpp"
#include "RapidXML/rapidxml_utils.hpp"

#include "types.h"
#include "LevelKeyboardHandler.h"
#include "Player.h"
#include "PlayerUpdater.h"
#include "LazyCameraManipulator.h"
#include "Cuboid.h"
#include "CollisionModel.h"
#include "HeadUpDisplay.h"
#include "Sky.h"
#include "Sound.h"

#define PHYSICS_WORLD_MIN           -1000, -1000, -1000
#define PHYSICS_WORLD_MAX           1000, 1000, 1000
#define PHYSICS_WORLD_GRAVITY       btVector3(0.0, 0.0, -40.0)

#define LEVEL_CAMERA_HOME_EYE       osg::Vec3(0.0, 1.0, 2.0)
#define LEVEL_CAMERA_HOME_CENTER    osg::Vec3(0.0, 10.0, 0.0)
#define LEVEL_CAMERA_HOME_UP        osg::Vec3(0.0, -10.0, 5.0)

class Level : public osg::Group
{
private:    
    osg::ref_ptr<osgShadow::ShadowedScene> _shadowedScene;
    osg::ref_ptr<HeadUpDisplay> _headUpDisplay;
    size_t _numDeaths;
    bool _reachedFinish;
    
    btDynamicsWorld *_physicsWorld;
	std::vector<float> _deadlyAltitudes;
    std::vector<osg::Vec3> _finishs;
    
    LevelKeyboardHandler *_keyboardHandler;
    
    void initializeLighting();
    void initializePhysicsWorld();
    void loadMapFromFile(const std::string &mapfile);
    
    osg::Vec3 getVectorFromXMLNode(const std::string &name, const rapidxml::xml_node<> &node) const;
            
public:
    Level(const std::string &mapfile);    
    
    void playerDied();
    
    std::vector<float> getDeadlyAltitudes() { return _deadlyAltitudes; }
    
    btDynamicsWorld *getPhysicsWorld() { return _physicsWorld; };

    void resetScene();
    
    void setReachedFinish(bool reachedFinish) { _reachedFinish = reachedFinish; }
    bool playerReachedFinish() { return _reachedFinish; }
    std::vector<osg::Vec3> getFinishs() { return _finishs; }
    LevelKeyboardHandler *getKeyboardHandler() { return _keyboardHandler; }
    
    HeadUpDisplay *getHeadUpDisplay() const;
    
    osgShadow::ShadowedScene* getShadowedScene() { return _shadowedScene; }
    
    size_t getNumDeaths() const;
    time_t getTime();
    
};

class LevelUpdater : public osg::NodeCallback
{
private:
    osg::BlendColor *_blendColor;
    Level *_level;
    float _previousStepTime;
    
public:
    LevelUpdater(Level *level);
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};