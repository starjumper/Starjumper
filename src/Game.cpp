#include "Game.h"

Game::Game(osgViewer::Viewer *viewer) :
	RenderingInstance(viewer)
{
    _running = false;
	_level = NULL;
	_player = new Player();
    
    _headUpDisplay = new HeadUpDisplay(_player);
    
    // initialize members
    _cameraManipulator = new LazyCameraManipulator();
    
    // setup manipulator to track the player
    _cameraManipulator->setTrackNode(_player->getNode());
    _cameraManipulator->setHomePosition(CAMERA_HOME_EYE, CAMERA_HOME_CENTER, CAMERA_HOME_UP);
    
    // set _cameraManipulator as manipulator for the scene
    getViewer()->setCameraManipulator(_cameraManipulator);

	// set Lighting Mode for the scene
	getViewer()->setLightingMode(osg::View::NO_LIGHT);
    
    _keyboardHandler = new GameKeyboardHandler(_player);
}

void Game::runLevel(const std::string &mapfile)
{
    _level = new Level(mapfile);
    _player->setDeadlyAltitudes(_level->getMinZValues());

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
    
    // register player controller
    _world->addAction(_player->getController());
    
    resetScene();
    
    // set world updater
    WorldUpdater *worldUpdater = new WorldUpdater(this);
    _level->getNode()->setUpdateCallback(worldUpdater); // TODO: find a better node than level (root does not work, produces segfault)	

    _headUpDisplay->resetTimer();

    _running = true;
}

void Game::initializeScene()
{
	// prepare shadowing
	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;

	shadowedScene->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	shadowedScene->setCastsShadowTraversalMask(CAST_SHADOW_MASK);

	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	shadowedScene->setShadowTechnique(sm.get());

    // add level and player to scene and setup heads up display
    shadowedScene->addChild(_level->getNode());
    shadowedScene->addChild(_player->getNode());
    getRootNode()->addChild(_headUpDisplay->getCamera());

    getRootNode()->addChild(shadowedScene);

	getRootNode()->addChild(createSkyBoxCubeMap());
    
    getRootNode()->addChild(_player->getEffectNode());
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

void Game::restartLevel()
{
    _player->getPlayerState()->setSpeed(0.0f);
    _player->toHomePosition();
    _cameraManipulator->resetCamera();
    _headUpDisplay->resetTimer();
}

btDynamicsWorld *Game::getWorld()
{
    return _world;
}

Player *Game::getPlayer()
{
    return _player;
}

void Game::prepare(osgViewer::Viewer *viewer)
{
    viewer->addEventHandler(_keyboardHandler);
}

void Game::cleanup(osgViewer::Viewer *viewer)
{    
    viewer->getEventHandlers().remove(_keyboardHandler);
    
    // clean up player position
    restartLevel();
}

bool Game::isRunning()
{
    return _running;
}

WorldUpdater::WorldUpdater(Game *game) :
    _game(game)
{
    _previousSimTime = 0.0f;
}

void WorldUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{    
    if(_game->isRunning())
    {
        if(_previousSimTime == 0.0f)
        {
            _previousSimTime = _game->getViewer()->getFrameStamp()->getSimulationTime();
        }
        else
        {
            double currentSimTime = _game->getViewer()->getFrameStamp()->getSimulationTime();
            _game->getWorld()->stepSimulation(currentSimTime - _previousSimTime);
            _previousSimTime = currentSimTime;
    
            // restart level if player is dead
            if(_game->getPlayer()->getPlayerState()->isDead())
            {
                _game->getPlayer()->getPlayerState()->beAlive();
                _game->restartLevel(); 
            }
        }
    }
}