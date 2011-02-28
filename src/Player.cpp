#include "Player.h"
#include "PlayerUpdater.h"

#include <iostream>

Player::Player(Lighting *playerLight) :
	_playerLight(playerLight)
{
    initializePlayerModel();
    initializePlayerPhysics();
    
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

void Player::toHomePosition()
{
    patPlayer->setPosition(osg::Vec3(0.0, 10.0, 5.0));
    
    btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(btVector3(0.0, 10.0, 5.0));
	_playerGhostObject->setWorldTransform(playerTransform);
}

osg::PositionAttitudeTransform *Player::getNode() const
{
    return _player;
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
