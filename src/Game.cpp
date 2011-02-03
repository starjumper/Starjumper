#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _level = new Level("resources/levels/level1.xml");
    _player = new Player();
    _controller = new PlayerController(_player);
    _headUpDisplay = new HeadUpDisplay(_player);
    	
    initializeScene();
    initializePhysics();
    
    // get level components and insert as rigid bodies into world
    std::vector<btRigidBody *> levelRBs = _level->getCollisionObjects();
    for(size_t i = 0; i < levelRBs.size(); ++i)
    {
        _world->addRigidBody(levelRBs[i]);
    }
    
    // add player ghost object to world
    _world->addCollisionObject(_player->getCollisionObject(),
                               btBroadphaseProxy::CharacterFilter,
                               btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    
    // register PlayerController
    _world->addAction(_player->getController());
}

void Game::initializeScene()
{
    // initialize members
    _cameraManipulator = new LazyCameraManipulator();
    
    // setup manipulator to track the player
    _cameraManipulator->setTrackNode(_player->getNode());
    _cameraManipulator->setHomePosition(CAMERA_HOME_EYE, CAMERA_HOME_CENTER, CAMERA_HOME_UP);
    
    // add level and player to scene and setup heads up display
    getRootNode()->addChild(_level->getNode());
    getRootNode()->addChild(_player->getNode());
    getRootNode()->addChild(_headUpDisplay->getCamera());
    
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
	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	
	_overlappingPairCache = sweepBP;

	_constraintSolver = new btSequentialImpulseConstraintSolver();
	
	// initialize world
	_world = new btDiscreteDynamicsWorld(_dispatcher,
	                                     _overlappingPairCache,
	                                     _constraintSolver,
	                                     _collisionConfiguration);

    // set the worlds gravity
    _world->setGravity(WORLD_GRAVITY);
}

void Game::prepare(osgViewer::Viewer *viewer)
{
    viewer->addEventHandler(_controller);
}

void Game::cleanup(osgViewer::Viewer *viewer)
{
    viewer->getEventHandlers().remove(_controller);
}