#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
	_level = new Level("resources/levels/level1.xml");
    _player = new Player();
    _controller = new PlayerController(_player);
    _headUpDisplay = new HeadUpDisplay(_player);
    _lighting = new Lighting();
    	
    initializeScene();
    initializePhysicsWorld();
    
    // get level components and insert as rigid bodies into world
    std::vector<btRigidBody *> levelRBs = _level->getCollisionObjects();
    for(std::vector<btRigidBody *>::iterator it = levelRBs.begin(); it != levelRBs.end(); ++it)
    {
        _world->addRigidBody(*it);
    }
    
    // add player ghost object to world
    _world->addCollisionObject(_player->getGhostObject(),
                               btBroadphaseProxy::CharacterFilter,
                               btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    
    // register PlayerController
    _world->addAction(_player->getController());
    
    resetScene();
    
    // set world updater
    WorldUpdater *worldUpdater = new WorldUpdater(this);
    _level->getNode()->setUpdateCallback(worldUpdater); // TODO: find a better node than level (root does not work, produces segfault)
}

void Game::initializeScene()
{
	// prepare shadowing
	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;

	shadowedScene->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	shadowedScene->setCastsShadowTraversalMask(CAST_SHADOW_MASK);

	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	shadowedScene->setShadowTechnique(sm.get());

	//int mapres = 1024;
	//sm->setTextureSize(osg::Vec2s(mapres,mapres));

    // initialize members
    _cameraManipulator = new LazyCameraManipulator();
    
    // setup manipulator to track the player
    _cameraManipulator->setTrackNode(_player->getNode());
    _cameraManipulator->setHomePosition(CAMERA_HOME_EYE, CAMERA_HOME_CENTER, CAMERA_HOME_UP);

    // add level and player to scene and setup heads up display
    shadowedScene->addChild(_level->getNode());
    shadowedScene->addChild(_player->getNode());
    getRootNode()->addChild(_headUpDisplay->getCamera());

	// add lighting
	osg::StateSet* rootStateSet = new osg::StateSet;
	getRootNode()->setStateSet(rootStateSet);

	shadowedScene->addChild(_lighting->getLights(rootStateSet));
    getRootNode()->addChild(shadowedScene);

    // set _cameraManipulator as manipulator for the scene
    getViewer()->setCameraManipulator(_cameraManipulator);
}

void Game::initializePhysicsWorld()
{
    // create CollisionConfiguration
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
    
    // define world extents
    btVector3 worldMin(WORLD_MIN);
	btVector3 worldMax(WORLD_MAX);
	
	// setup overlapping pair cache
	btAxisSweep3 *sweepBP = new btAxisSweep3(worldMin,worldMax);
	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	_overlappingPairCache = sweepBP;

    // create default constraint solver
	_constraintSolver = new btSequentialImpulseConstraintSolver();
	
	// initialize world
	_world = new btDiscreteDynamicsWorld(_dispatcher,
	                                     _overlappingPairCache,
	                                     _constraintSolver,
	                                     _collisionConfiguration);

    // set the worlds gravity
    _world->setGravity(WORLD_GRAVITY);
}

void Game::resetScene()
{
    if(!_world)
        return;
    
    btCollisionObjectArray collisionObjects = _world->getCollisionObjectArray();
    size_t collisionObjectCount = _world->getNumCollisionObjects();
    
    for(size_t i = 0; i < collisionObjectCount; ++i)
    {
        btCollisionObject *collisionObject = collisionObjects[i];
        btRigidBody* body = btRigidBody::upcast(collisionObject);
        
        if(!body)
            continue;
        
        if(body->getMotionState())
        {
            btDefaultMotionState* motionState = (btDefaultMotionState*)body->getMotionState();
			motionState->m_graphicsWorldTrans = motionState->m_startWorldTrans;
			body->setCenterOfMassTransform(motionState->m_graphicsWorldTrans);
			collisionObject->setInterpolationWorldTransform(motionState->m_startWorldTrans);
			collisionObject->forceActivationState(ACTIVE_TAG);
			collisionObject->activate();
			collisionObject->setDeactivationTime(0);
        }
        
        if (body && !body->isStaticObject())
		{
			btRigidBody::upcast(collisionObject)->setLinearVelocity(btVector3(0,0,0));
			btRigidBody::upcast(collisionObject)->setAngularVelocity(btVector3(0,0,0));
		}
    }
    
    _world->getBroadphase()->resetPool(_world->getDispatcher());
	_world->getConstraintSolver()->reset();
}

btDynamicsWorld *Game::getWorld()
{
    return _world;
}

void Game::prepare(osgViewer::Viewer *viewer)
{
    viewer->addEventHandler(_controller);
}

void Game::cleanup(osgViewer::Viewer *viewer)
{
    viewer->getEventHandlers().remove(_controller);
}

WorldUpdater::WorldUpdater(Game *game) :
    _game(game)
{
    _previousSimTime = _game->getViewer()->getFrameStamp()->getSimulationTime();
}

void WorldUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    double currentSimTime = _game->getViewer()->getFrameStamp()->getSimulationTime();
    _game->getWorld()->stepSimulation(currentSimTime - _previousSimTime);
    _previousSimTime = currentSimTime;
}