#include "Level.h"

extern osgViewer::Viewer viewer;

Level::Level(const std::string &mapfile)
{
    initializePhysicsWorld();
    
    // load map from file
    loadMapFromFile(mapfile);
    
    // add player to level
    addChild(Player::getInstance());
    
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
    
    // set _cameraManipulator as manipulator for the scene
    viewer.setCameraManipulator(cameraManipulator);
    
    LevelUpdater *stepCallback = new LevelUpdater(this);
    setUpdateCallback(stepCallback);
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
            collisionObject = new Cuboid(   getVectorFromXMLNode("position", *it), 
                                            getVectorFromXMLNode("size", *it));
        else if(strcmp(it->name(), "tunnel") == 0)
            ;
        else if(strcmp(it->name(), "cuboidtunnel") == 0)
            ;
        else if(strcmp(it->name(), "goal") == 0)
            ;
		else
            throw std::runtime_error("Error: Unknown element \'" + std::string(it->name()) + "\' in level file!");
            
        if(collisionObject != 0)
        {
            addChild(collisionObject);
            _physicsWorld->addRigidBody(collisionObject->getRigidBody());
        }
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

////////////// World updater for stepping //////////////

LevelUpdater::LevelUpdater(Level *level) :
    _level(level),
    _previousStepTime(0.0f)
{
}

void LevelUpdater::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    double currentStepTime = viewer.getFrameStamp()->getSimulationTime();
    _level->getPhysicsWorld()->stepSimulation(currentStepTime - _previousStepTime, 0);
    _previousStepTime = currentStepTime;
}
