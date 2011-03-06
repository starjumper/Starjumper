#include "Player.h"
#include "PlayerUpdater.h"

Player::Player(Lighting *playerLight) :
	_playerLight(playerLight)
{
    initializePlayerModel();
    initializePlayerPhysics();
    initializePlayerEffects();
    
    _playerState = new PlayerState();
}

Player::~Player()
{
    delete _playerState;
}

void Player::initializePlayerModel()
{
    _player = new osg::PositionAttitudeTransform;
    
    osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);
    if(!playerModel)
    {
        throw std::runtime_error("Unable to load player model file!");
    }
    
    patPlayer = new osg::PositionAttitudeTransform;

    patPlayer->addChild(playerModel);
    patPlayer->setScale(PLAYER_SCALE);
    patPlayer->setAttitude(PLAYER_ATTITUDE);
    patPlayer->setPosition(osg::Vec3(0.0, 10.0, 5.0));

    _player->setNodeMask(CAST_SHADOW_MASK);
    _player->addChild(patPlayer);
	
    PlayerUpdater *p_up = new PlayerUpdater(this);
    patPlayer->setUpdateCallback(p_up);
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
    //_leftSteer = ParticleEffectFactory::createSteerEngineEffect();
    //_rightSteer = ParticleEffectFactory::createSteerEngineEffect();
    
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
    
   /* _leftSteer->setScale(PLAYER_SCALE);
    _leftSteer->setPosition(osg::Vec3(-1.8, 5.0, 0.7));
    _leftSteer->setAttitude(osg::Quat(
            osg::DegreesToRadians(180.0),
            osg::Vec3(1.0, 0.0, 0.0)
        ));
    
    _rightSteer->setScale(PLAYER_SCALE);
    _rightSteer->setPosition(osg::Vec3(1.8, 4.0, 0.7));
/*    _leftSteer->setAttitude(osg::Quat(
            osg::DegreesToRadians(90.0),
            osg::Vec3(1.0, 0.0, 0.0)
        ));*/
    
    patPlayer->addChild(_mainEngine);
    patPlayer->addChild(_leftEngine);
    patPlayer->addChild(_rightEngine);
    //patPlayer->addChild(_leftSteer);
    //patPlayer->addChild(_rightSteer);
    
    // add the other components to the scene
    _particleEffects->addChild(_mainEngine->getEffectRoot());
    _particleEffects->addChild(_leftEngine->getEffectRoot());
    _particleEffects->addChild(_rightEngine->getEffectRoot());
    //_particleEffects->addChild(_leftSteer->getEffectRoot());
    //_particleEffects->addChild(_rightSteer->getEffectRoot());
}

void Player::toHomePosition()
{
    patPlayer->setPosition(osg::Vec3(0.0, 10.0, 5.0));
    
    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(btVector3(0.0, 10.0, 5.0));
	_playerGhostObject->setWorldTransform(playerTransform);
    _mainEngine->clearParticles();
    _leftEngine->clearParticles();
    _rightEngine->clearParticles();
}

osg::PositionAttitudeTransform *Player::getNode() const
{
    return _player;
}

osg::Group *Player::getEffectNode() const
{
    return _particleEffects;
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
    patPlayer->setPosition(position);
	_playerLight->setLightPosition(position + osg::Vec3(0.0, 0.0, 2.0f));
}

void Player::setAngles(const float angleX, const float angleY, const float angleZ)
{
    patPlayer->setAttitude(osg::Quat(
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