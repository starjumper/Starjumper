#pragma once

#include <vector>

#include <osg/Group>

#include <btBulletDynamicsCommon.h>

#include "RenderingInstance.h"
#include "LazyCameraManipulator.h"
#include "Level.h"
#include "Player.h"
#include "PlayerController.h"
#include "HeadUpDisplay.h"

#define WORLD_MIN          -1000, -1000, -1000
#define WORLD_MAX          1000, 1000, 1000
#define WORLD_GRAVITY      btVector3(0.0, 0.0, -9.81)
#define CAMERA_HOME_EYE    osg::Vec3(0.0, -5.0, 4.0)
#define CAMERA_HOME_CENTER osg::Vec3(0.0, 20.0, 0.0)
#define CAMERA_HOME_UP     osg::Vec3(0.0, -10.0, 10.0)

class Game : public RenderingInstance
{
private:
    Level *_level;
    Player *_player;
    PlayerController *_controller;
    LazyCameraManipulator *_cameraManipulator;
    HeadUpDisplay *_headUpDisplay;
    
    btDynamicsWorld *_world;
    btDefaultCollisionConfiguration *_collisionConfiguration;
    btCollisionDispatcher *_dispatcher;
    btBroadphaseInterface *_overlappingPairCache;
    btConstraintSolver *_constraintSolver;
    
    void initializeScene();
    void initializePhysics();
    
public:
    Game(osgViewer::Viewer *viewer);
    
    virtual void prepare(osgViewer::Viewer *viewer);
    virtual void cleanup(osgViewer::Viewer *viewer);
};