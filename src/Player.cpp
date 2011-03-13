#include "Player.h"
#include "PlayerUpdater.h"

Player::Player()
{
    initializePlayerModel();
    initializePlayerPhysics();
    initializePlayerEffects();
    
    _playerState = new PlayerState();
	
}

Player::~Player()
{
    delete _deadlyAltitudes;
    delete _playerState;
}

void Player::initializePlayerModel()
{
    osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);
    if(!playerModel)
    {
        throw std::runtime_error("Unable to load player model file!");
    }
    
    _playerPAT = new osg::PositionAttitudeTransform;
    addChild(_playerPAT);

    _playerPAT->addChild(playerModel);
    _playerPAT->setScale(PLAYER_SCALE);
    _playerPAT->setAttitude(PLAYER_ATTITUDE);
    _playerPAT->setPosition(osg::Vec3(0.0, 10.0, 5.0));

	// add Light for player shadow
	osg::Light *playerLight = new osg::Light();
	playerLight->setLightNum(3);
	osg::LightSource *lightSource = new osg::LightSource;   
	lightSource->setLight(playerLight);
	osg::StateSet *stateset = new osg::StateSet;
	lightSource->setStateSetModes(*stateset, osg::StateAttribute::ON);
	playerLight->setPosition(osg::Vec4(osg::Vec3(0.0, 10.0, 12.0),1.0));
	playerLight->setDiffuse(osg::Vec4(1.0,0.0,0.0,0.5));
	playerLight->setAmbient(osg::Vec4(1.0,1.0,1.0,1.0));

	_playerPAT->addChild(lightSource);
    _playerPAT->setNodeMask(CAST_SHADOW_MASK);
	
    PlayerUpdater *playerUpdater = new PlayerUpdater();
    setUpdateCallback(playerUpdater);
}

void Player::initializePlayerPhysics()
{
    _playerGhostObject = new btPairCachingGhostObject;
    
    // use a btBoxShape as collision shape for the player
    btBoxShape *boundingBox = new btBoxShape(PLAYER_BBOX_EXTENTS);
    _playerGhostObject->setCollisionShape(boundingBox);
    _playerGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    
    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(btVector3(0.0, 10.0, 5.0));
	_playerGhostObject->setWorldTransform(playerTransform);
	
	_playerController = new KinematicCharacterController(_playerGhostObject, boundingBox, btScalar(0.1), 2);
    _playerController->setFallSpeed(0.1);
}

void Player::initializePlayerEffects()
{
    _particleEffects = new osg::Group;
    
    _mainEngine = ParticleEffectFactory::createRearEngineEffect();
    _leftEngine = ParticleEffectFactory::createRearEngineEffect();
    _rightEngine = ParticleEffectFactory::createRearEngineEffect();
    
    // position the particle effect emitters
    _mainEngine->setScale(PLAYER_SCALE);
    _mainEngine->setPosition(osg::Vec3(0.0, 6.5, 1.4));
    _mainEngine->setAttitude(osg::Quat(
            osg::DegreesToRadians(90.0),
            osg::Vec3(1.0, 0.0, 0.0)
        ));
    
    _leftEngine->setScale(PLAYER_SCALE);
    _leftEngine->setPosition(osg::Vec3(-1.8, 6.0, 0.9));
    _leftEngine->setAttitude(osg::Quat(
            osg::DegreesToRadians(90.0),
            osg::Vec3(1.0, 0.0, 0.0)
        ));
        
    _rightEngine->setScale(PLAYER_SCALE);
    _rightEngine->setPosition(osg::Vec3(1.8, 6.0, 0.9));
    _rightEngine->setAttitude(osg::Quat(
            osg::DegreesToRadians(90.0),
            osg::Vec3(1.0, 0.0, 0.0)
        ));
    
    _playerPAT->addChild(_mainEngine);
    _playerPAT->addChild(_leftEngine);
    _playerPAT->addChild(_rightEngine);
    
    // add the other components to the scene
    addChild(_mainEngine->getEffectRoot());
    addChild(_leftEngine->getEffectRoot());
    addChild(_rightEngine->getEffectRoot());
}

void Player::toHomePosition()
{
    _playerPAT->setPosition(osg::Vec3(0.0, 10.0, 5.0));
    
    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(btVector3(0.0, 10.0, 5.0));
	_playerGhostObject->setWorldTransform(playerTransform);
    _mainEngine->clearParticles();
    _leftEngine->clearParticles();
    _rightEngine->clearParticles();
}

osg::PositionAttitudeTransform *Player::getPlayerPAT() const
{
    return _playerPAT;
}

btPairCachingGhostObject *Player::getGhostObject() const
{
    return _playerGhostObject;
}

KinematicCharacterController *Player::getController() const
{
    return _playerController;
}

PlayerState *Player::getPlayerState() const
{
    return _playerState;
}

void Player::setPosition(const osg::Vec3 position)
{
    _playerPAT->setPosition(position);
}

void Player::setAngles(const float angleX, const float angleY, const float angleZ)
{
    _playerPAT->setAttitude(osg::Quat(
        osg::DegreesToRadians(angleX), osg::Vec3(1.0,0.0,0.0),
        osg::DegreesToRadians(angleY), osg::Vec3(0.0,1.0,0.0),
        osg::DegreesToRadians(angleZ), osg::Vec3(0.0,0.0,1.0)));
}

void Player::setDeadlyAltitudes(const std::vector<float> *values)
{
    _deadlyAltitudes = values;
}

const std::vector<float> *Player::getDeadlyAltitudes()
{
    return _deadlyAltitudes;
}

void Player::setEnginesAccelerating(const float speed)
{
    _mainEngine->setRate(250 * speed);
    _leftEngine->setRate(250 * speed);
    _rightEngine->setRate(250 * speed);
    
    _mainEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.8f));
    _leftEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.9f));
    _rightEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.8f));
    
    _mainEngine->setSize(0.1f * speed);
    _leftEngine->setSize(0.1f * speed);
    _rightEngine->setSize(0.1f * speed);
}

void Player::setEnginesDecelerating(const float speed)
{
    _mainEngine->setRate(250 * speed);
    _leftEngine->setRate(250 * speed);
    _rightEngine->setRate(250 * speed);
    
    _mainEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.5f));
    _leftEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.4f));
    _rightEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.4f));
    
    _mainEngine->setSize(0.1f * speed);
    _leftEngine->setSize(0.1f * speed);
    _rightEngine->setSize(0.1f * speed);
}