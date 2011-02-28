#pragma once

#include <vector>

#include <osg/Group>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/StandardShadowMap>

#include <btBulletDynamicsCommon.h>

#include "types.h"
#include "RenderingInstance.h"
#include "LazyCameraManipulator.h"
#include "Level.h"
#include "Lighting.h"
#include "Player.h"
#include "GameKeyboardHandler.h"
#include "HeadUpDisplay.h"

#define WORLD_MIN          -1000, -1000, -1000
#define WORLD_MAX          1000, 1000, 1000
#define WORLD_GRAVITY      btVector3(0.0, 0.0, -40.0)
#define CAMERA_HOME_EYE    osg::Vec3(0.0, 1.0, 2.0)
#define CAMERA_HOME_CENTER osg::Vec3(0.0, 10.0, 0.0)
#define CAMERA_HOME_UP     osg::Vec3(0.0, -10.0, 5.0)

class Game : public RenderingInstance
{
private:
    Level *_level;
    Player *_player;
    GameKeyboardHandler *_keyboardHandler;
    LazyCameraManipulator *_cameraManipulator;
    HeadUpDisplay *_headUpDisplay;
	Lighting *_lighting;
	btDynamicsWorld *_world;
    
    btDefaultCollisionConfiguration *_collisionConfiguration;
    btCollisionDispatcher *_dispatcher;
    btBroadphaseInterface *_overlappingPairCache;
    btConstraintSolver *_constraintSolver;
    
    void initializeScene();
    void initializePhysicsWorld();
    void resetScene();
    
public:
    Game(osgViewer::Viewer *viewer);
    
    btDynamicsWorld *getWorld();
    Player *getPlayer();

    void restartLevel();

    virtual void prepare(osgViewer::Viewer *viewer);
    virtual void cleanup(osgViewer::Viewer *viewer);
};

class WorldUpdater : public osg::NodeCallback
{
private:
    Game *_game;
    double _previousSimTime;
    
public:
    WorldUpdater(Game *game);
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};