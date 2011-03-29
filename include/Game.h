#pragma once

#include <vector>

#include <osg/Group>
#include <osg/StateAttribute>
#include <osg/BlendFunc>
#include <osg/BlendColor>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgAudio/FileStream.h>
#include <osgAudio/SoundManager.h>
#include <osgAudio/SoundState.h>
#include <osgAudio/SoundRoot.h>

#include <btBulletDynamicsCommon.h>

#include "types.h"
#include "RenderingInstance.h"
#include "LazyCameraManipulator.h"
#include "Level.h"
#include "Player.h"
#include "GameKeyboardHandler.h"
#include "HeadUpDisplay.h"
#include "Skybox.h"

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
    osg::ref_ptr<Player> _player;
    osg::ref_ptr<GameKeyboardHandler> _keyboardHandler;
    LazyCameraManipulator *_cameraManipulator;
    HeadUpDisplay *_headUpDisplay;
	btDynamicsWorld *_world;
    
    btDefaultCollisionConfiguration *_collisionConfiguration;
    btCollisionDispatcher *_dispatcher;
    btBroadphaseInterface *_overlappingPairCache;
    btConstraintSolver *_constraintSolver;
    
    bool _running;
    bool _levelFinished;
    
    void initializeScene();
    void initializePhysicsWorld();
    void resetScene();
    
public:
    Game(osgViewer::Viewer *viewer);
    
    btDynamicsWorld *getWorld();
    Player *getPlayer();

    void restartLevel();
    bool isRunning();
    
    void runLevel(const std::string &mapfile);    
    void setLevelFinished(bool levelFinished);
    bool getLevelFinished();

    virtual void prepare(osgViewer::Viewer *viewer);
    virtual void cleanup(osgViewer::Viewer *viewer);
};

class WorldUpdater : public osg::NodeCallback
{
private:
    Game *_game;
    double _previousSimTime;
    osg::BlendColor *_blendColor;
    
public:
    WorldUpdater(Game *game);
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};