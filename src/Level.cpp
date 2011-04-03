#include "Level.h"

extern osgViewer::Viewer viewer;

Level::Level(const std::string &mapfile) :
    _numDeaths(0),
    _reachedFinish(false)
{
    _shadowedScene = new osgShadow::ShadowedScene;
    _shadowedScene->setReceivesShadowTraversalMask(RECEIVE_SHADOW_MASK);
	_shadowedScene->setCastsShadowTraversalMask(CAST_SHADOW_MASK);
	_shadowedScene->setShadowTechnique(new osgShadow::ShadowMap);
 
    addChild(_shadowedScene);

    _headUpDisplay = new HeadUpDisplay();
    addChild(_headUpDisplay->getCamera());
    addChild((new Sky())->getCamera());
    
    initializePhysicsWorld();
    
    // load map from file
    loadMapFromFile(mapfile);
    
    // add player to level
    _shadowedScene->addChild(Player::getInstance());
    addChild(Player::getInstance()->getParticleEffects());
	
    // add player ghost object to world
    _physicsWorld->addCollisionObject(Player::getInstance()->getGhostObject(),
                               btBroadphaseProxy::CharacterFilter,
                               btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
                               
    // register player controller
    _physicsWorld->addAction(Player::getInstance()->getController());
    
    // initialize members
    LazyCameraManipulator *cameraManipulator = new LazyCameraManipulator();
    
    // setup manipulator to track the player
    cameraManipulator->setTrackNode(Player::getInstance());
    cameraManipulator->setHomePosition(LEVEL_CAMERA_HOME_EYE, LEVEL_CAMERA_HOME_CENTER, LEVEL_CAMERA_HOME_UP);

    // player must be updated after physic is updated
    Player::getInstance()->setUpdateCallback(new PlayerUpdater());
    
    // set _cameraManipulator as manipulator for the scene
    viewer.setCameraManipulator(cameraManipulator);
                
    LevelUpdater *stepCallback = new LevelUpdater(this);
    setUpdateCallback(stepCallback);
    
    // player keyboard control
    _keyboardHandler = new LevelKeyboardHandler();
    viewer.addEventHandler(_keyboardHandler);
    
    initializeLighting();
    
    Sound::switchBackgroundMusic(LEVEL_MUSIC_FILE, "GameMusic");    
}

void Level::playerDied()
{
    _headUpDisplay->resetTimer();
    _numDeaths++;
}

void Level::initializeLighting()
{    
    // add Light for player shadow
	osg::Light *light = new osg::Light();
	light->setLightNum(3);
	
	osg::LightSource *lightSource = new osg::LightSource;   
	lightSource->setLight(light);
	
	osg::StateSet *stateset = new osg::StateSet;
	lightSource->setStateSetModes(*stateset, osg::StateAttribute::ON);
	
	light->setPosition(osg::Vec4(osg::Vec3(0.0, 10.0, 1000.0),1.0));
	light->setDiffuse(osg::Vec4(1.0,0.0,0.0,0.5));
	light->setAmbient(osg::Vec4(1.0,1.0,1.0,1.0));

	_shadowedScene->addChild(lightSource);
}

void Level::loadMapFromFile(const std::string &mapfile)
{
    // load XML document
    rapidxml::file<> mf(mapfile.c_str());
    rapidxml::xml_document<> xml_doc;
    xml_doc.parse<0>(mf.data());
    
    // parse XML document
    for(rapidxml::node_iterator<char> it(xml_doc.first_node()); it.dereference() != NULL; ++it)
    {
        CollisionObject *collisionObject = 0;
        
        if(strcmp(it->name(), "cuboid") == 0)
        {
            osg::Vec3 from = getVectorFromXMLNode("position", *it);
            osg::Vec3 size = getVectorFromXMLNode("size", *it);
            
            if(it->first_attribute("type") == 0)
                collisionObject = new DefaultCuboid(from, size);
            else if(std::string(it->first_attribute("type")->value()) == "accelerate")
                collisionObject = new AccelerationCuboid(from, size);
            else if(std::string(it->first_attribute("type")->value()) == "decelerate")
                collisionObject = new DecelerationCuboid(from, size);
            else
                collisionObject = new DefaultCuboid(from, size);            

            int yBucketIndex = (int)((from.y() + size.y()) / 20.0f);
         
            while((int)_deadlyAltitudes.size() <= yBucketIndex)
                _deadlyAltitudes.push_back(from.z());

            // if current cuboid is lower then z -> adjust bucket value
            if(from.z() < _deadlyAltitudes[yBucketIndex])
                _deadlyAltitudes[yBucketIndex] = from.z();    
                
            _shadowedScene->addChild(collisionObject);                                   
        }
        else if(strcmp(it->name(), "tunnel") == 0)
        {
        	collisionObject = new Tunnel(getVectorFromXMLNode("position", *it), atof(it->first_attribute("length")->value()));
            _shadowedScene->addChild(((CollisionModel *)collisionObject)->getNode()); 
        }
        else if(strcmp(it->name(), "cuboidtunnel") == 0)
        {
            collisionObject = new CuboidTunnel(getVectorFromXMLNode("position", *it), atof(it->first_attribute("length")->value()));
            _shadowedScene->addChild(((CollisionModel *)collisionObject)->getNode());
        }
        else if(strcmp(it->name(), "finish") == 0)
        {
            osg::Vec3 position = getVectorFromXMLNode("position", *it);
            collisionObject = new Finish(position);
            _finishs.push_back(position);
            _shadowedScene->addChild(((CollisionModel *)collisionObject)->getNode());        	            
        }
		else
            throw std::runtime_error("Error: Unknown element \'" + std::string(it->name()) + "\' in level file!");
            
        if(collisionObject != 0 && strcmp(it->name(), "finish") != 0)
            _physicsWorld->addRigidBody(collisionObject->getRigidBody());
    }
}

void Level::initializePhysicsWorld()
{
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btBroadphaseInterface *overlappingPairCache;
    btConstraintSolver *constraintSolver;
    
    // create CollisionConfiguration
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    // define world extents
    btVector3 worldMin(PHYSICS_WORLD_MIN);
	btVector3 worldMax(PHYSICS_WORLD_MAX);
	
	// setup overlapping pair cache
	btAxisSweep3 *sweepBP = new btAxisSweep3(worldMin, worldMax);
	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	overlappingPairCache = sweepBP;

    // create default constraint solver
	constraintSolver = new btSequentialImpulseConstraintSolver();
	
	// initialize world
	_physicsWorld = new btDiscreteDynamicsWorld(dispatcher,
	                                    overlappingPairCache,
	                                    constraintSolver,
	                                    collisionConfiguration);

    // set the worlds gravity
    _physicsWorld->setGravity(PHYSICS_WORLD_GRAVITY);
}

void Level::resetScene()
{
    if(!_physicsWorld)
        return;
    
    btCollisionObjectArray collisionObjects = _physicsWorld->getCollisionObjectArray();
    size_t collisionObjectCount = _physicsWorld->getNumCollisionObjects();
    
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
    
    _physicsWorld->getBroadphase()->resetPool(_physicsWorld->getDispatcher());
	_physicsWorld->getConstraintSolver()->reset();
	
    delete _physicsWorld;
}

osg::Vec3 Level::getVectorFromXMLNode(const std::string &name, const rapidxml::xml_node<> &node) const
{
    rapidxml::xml_node<> *vectorNode = node.first_node(name.c_str());
        
    if(!vectorNode)
    {
        throw std::runtime_error("Error: Level element missing vector node!");
    }
        
    float x, y, z;
    
    try
    {
        x = atof(vectorNode->first_attribute("x")->value());
        y = atof(vectorNode->first_attribute("y")->value());
        z = atof(vectorNode->first_attribute("z")->value());
    }
    catch(...)
    {
        throw std::runtime_error("Error: " + name + " node missing either x, y or z attribute!");
    }
    
    return osg::Vec3(x, y, z);   
}

time_t Level::getTime()
{
    return _headUpDisplay->getFinalTime();
}


HeadUpDisplay *Level::getHeadUpDisplay() const
{
    return _headUpDisplay;
}

size_t Level::getNumDeaths() const
{
    return _numDeaths;
}

////////////// World updater for stepping //////////////

LevelUpdater::LevelUpdater(Level *level) :
    _level(level),
    _previousStepTime(viewer.getFrameStamp()->getSimulationTime())
{
    _blendColor = new osg::BlendColor(osg::Vec4(1, 1, 1, 1));
    
    _level->getShadowedScene()->getOrCreateStateSet()->setAttributeAndModes(_blendColor, osg::StateAttribute::ON);
}

#include <iostream>

void LevelUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{    
    double currentStepTime = viewer.getFrameStamp()->getSimulationTime();
    
    // compensate error arising from the osg::Viewer resetting its SimulationTime
    /*if(fabs(currentStepTime - _previousStepTime) > 0.1f)
        _previousStepTime = currentStepTime - 0.05;*/
    
    if(_previousStepTime <= 0.1f)
    {
        Player::getInstance()->resetPosition();
        ((LazyCameraManipulator *)viewer.getCameraManipulator())->resetCamera();        
    }
    else
    {
        std::cout << (int)((currentStepTime - _previousStepTime) * 60.0f) + 1 << std::endl;

        _level->getPhysicsWorld()->stepSimulation(currentStepTime - _previousStepTime, 
                    (int)((currentStepTime - _previousStepTime) * 60.0f) + 1);        
    }
       
    _previousStepTime = currentStepTime;
    
    // player dies when falling too low
    {
        btVector3 position = Player::getInstance()->getController()->getGhostObject()->getWorldTransform().getOrigin();
        int yBucketIndex = (int)(position.y() / 20.0f);
    
        if(yBucketIndex >= _level->getDeadlyAltitudes().size())
            yBucketIndex = _level->getDeadlyAltitudes().size() - 1;

        float minimum = min(_level->getDeadlyAltitudes()[yBucketIndex], 
                            (_level->getDeadlyAltitudes())[yBucketIndex + 1]);

        if(position.z() < (minimum - 5.0f))
        {
            Player::getInstance()->resetPosition();
            ((LazyCameraManipulator *)viewer.getCameraManipulator())->resetCamera();
            _level->playerDied();
        }
    }
    
    // fade out when level is finished
    osg::Vec4 constantBlendColor = _blendColor->getConstantColor();
    float alpha = constantBlendColor.a();
    
    // player reached finish
    {
        osg::Vec3 position = Player::getInstance()->getPosition();
        std::vector<osg::Vec3> finishs = _level->getFinishs();

        for(size_t i = 0; i < finishs.size(); i++)
        {
            float maxDistance = 1.0f;
            osg::Vec3 diff = position - finishs[i];
            
            if(diff.x() < maxDistance && diff.x() > -maxDistance &&
                diff.y() < maxDistance && diff.y() > -maxDistance &&
                diff.z() < maxDistance && diff.z() > -maxDistance)
                {
                    if(alpha == 1.0f)
                    {
                        alpha -= 0.01f;
                        _level->getHeadUpDisplay()->stopTimer();
                        viewer.getEventHandlers().remove(_level->getKeyboardHandler());
                        ((LazyCameraManipulator *)viewer.getCameraManipulator())->fadeOut();
                        Player::getInstance()->getPlayerState()->setRequestAccelerate(false);
                        Player::getInstance()->getPlayerState()->setRequestDecelerate(true);
                    }                    
                }
        }
        
        if(alpha < 1.0f)
        {
            alpha -= 0.01f;

            if(alpha <= 0.0f)
            {
                alpha = 0.0f;                 
                _level->setReachedFinish(true);
            }

            constantBlendColor[3] = alpha;

            _blendColor->setConstantColor(constantBlendColor);
        }
    }
    
    traverse(node, nv);
}
