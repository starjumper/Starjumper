#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _level = new Level("resources/levels/level1.xml");
    _player = new Player();
    
    initializeScene();
    initializePhysics();
}

void Game::initializeScene()
{
    // initialize members
    _cameraManipulator = new LazyCameraManipulator();
    
    // setup manipulator to track the player
    _cameraManipulator->setTrackNode(_player->getNode());
    _cameraManipulator->setHomePosition(CAMERA_HOME_EYE, CAMERA_HOME_CENTER, CAMERA_HOME_UP);
    
    // add level and player to scene
    getRootNode()->addChild(_level->getNode());
    getRootNode()->addChild(_player->getNode());
    
    // set _cameraManipulator as manipulator for the scene
    getViewer()->setCameraManipulator(_cameraManipulator);
}

void Game::initializePhysics()
{
    // create CollisionConfiguration
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    
    // define world extents
    btVector3 worldMin(WORLD_MIN);
	btVector3 worldMax(WORLD_MAX);
	
	// setup sweep axis
	btAxisSweep3 *sweepBP = new btAxisSweep3(worldMin,worldMax);
	
	_overlappingPairCache = sweepBP;

	_constraintSolver = new btSequentialImpulseConstraintSolver();
	
	// initialize world
	_world = new btDiscreteDynamicsWorld(_dispatcher,
	                                     _overlappingPairCache,
	                                     _constraintSolver,
	                                     _collisionConfiguration);
}
