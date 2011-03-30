#include "Player.h"
#include "PlayerUpdater.h"

Player *Player::_instance = NULL;

Player::Player() :
    _playerState(new PlayerState())
{    
    loadPlayerModel();
    setScale(PLAYER_SCALE);
    setAttitude(PLAYER_ATTITUDE);
    initializePhysics(); 
    initializePlayerEffects();   
    resetPosition();
    setNodeMask(CAST_SHADOW_MASK);
}

Player *Player::getInstance()
{
    if (!_instance)
        _instance = new Player();
  
    return _instance;
}

void Player::loadPlayerModel()
{
    osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);

    if(!playerModel)
        throw std::runtime_error("Unable to load player model file!");
    
    addChild(playerModel);
}

void Player::initializePhysics()
{
    _playerGhostObject = new btPairCachingGhostObject;
    
    // use a btBoxShape as collision shape for the player
    btBoxShape *boundingBox = new btBoxShape(PLAYER_BBOX_EXTENTS);
    _playerGhostObject->setCollisionShape(boundingBox);
    _playerGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    
    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(osgbBullet::asBtVector3(PLAYER_HOME_POSITION));
	_playerGhostObject->setWorldTransform(playerTransform);
	
	_playerController = new KinematicCharacterController(_playerGhostObject, boundingBox, btScalar(0.1), 2);
    _playerController->setFallSpeed(0.1);
}

void Player::initializePlayerEffects()
{    
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
    
    addChild(_mainEngine);
    addChild(_leftEngine);
    addChild(_rightEngine);
    
    // add the other components to the scene
    _particleEffects = new osg::Group;
    
    _particleEffects->addChild(_mainEngine->getEffectRoot());
    _particleEffects->addChild(_leftEngine->getEffectRoot());
    _particleEffects->addChild(_rightEngine->getEffectRoot());
}

void Player::setEngines(const float speed, bool accelerating)
{
    _mainEngine->setRate(250 * speed);
    _leftEngine->setRate(250 * speed);
    _rightEngine->setRate(250 * speed);
    
    if(accelerating)
    {
        _mainEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.8f));
        _leftEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.9f));
        _rightEngine->setColor(osg::Vec4(1.0f, 0.7f, 0.1f, 0.8f));
    }
    else
    {
        _mainEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.5f));
        _leftEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.4f));
        _rightEngine->setColor(osg::Vec4(0.0f, 0.7f, 1.0f, 0.4f));
    }
    
    _mainEngine->setSize(0.1f * speed);
    _leftEngine->setSize(0.1f * speed);
    _rightEngine->setSize(0.1f * speed);
}

void Player::resetPosition()
{
    setPosition(PLAYER_HOME_POSITION);

    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(btVector3(0.0, 10.0, 5.0));

	_playerGhostObject->setWorldTransform(playerTransform);
    _mainEngine->clearParticles();
    _leftEngine->clearParticles();
    _rightEngine->clearParticles();
}

void Player::setAngles(const float angleX, const float angleY, const float angleZ)
{
    setAttitude(osg::Quat(
        osg::DegreesToRadians(angleX), osg::Vec3(1.0,0.0,0.0),
        osg::DegreesToRadians(angleY), osg::Vec3(0.0,1.0,0.0),
        osg::DegreesToRadians(angleZ), osg::Vec3(0.0,0.0,1.0)));
}
