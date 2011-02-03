#include "Player.h"
#include "PlayerUpdater.h"

#include <iostream>

Player::Player()
{
    initializePlayerModel();
    initializePlayerPhysics();
}

void Player::initializePlayerModel()
{
    _player = new osg::PositionAttitudeTransform;
    
    osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);
    if(!playerModel)
    {
        throw std::runtime_error("Unable to load player model file!");
    }
    
    osg::PositionAttitudeTransform *patPlayer = new osg::PositionAttitudeTransform;

    patPlayer->addChild(playerModel);
    patPlayer->setScale(PLAYER_SCALE);
    patPlayer->setAttitude(PLAYER_ATTITUDE);
    patPlayer->setPosition(osg::Vec3(0.0, 10.0, 5.0));

    _player->addChild(patPlayer);
    
    PlayerUpdater *p_up = new PlayerUpdater(this);
    patPlayer->setUpdateCallback(p_up);
    
    _moveLeft = false;
    _moveRight = false;
    _accelerate = false;
    _decelerate = false;
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
	playerTransform.setOrigin(btVector3(0.0, 4.0, 0.0));
	_playerGhostObject->setWorldTransform(playerTransform);
	
	_playerController = new btKinematicCharacterController(_playerGhostObject, boundingBox, btScalar(0.35));
}

void Player::moveLeft(bool &keyState)
{
    std::cout << "moveLeft" << std::endl;
    _moveLeft = keyState;
}

void Player::moveRight(bool &keyState)
{
    std::cout << "moveRight" << std::endl;
    _moveRight = keyState;
}

void Player::accelerate(bool &keyState)
{
    std::cout << "accelerate" << std::endl;
    _accelerate = keyState;
}

void Player::decelerate(bool &keyState)
{
    std::cout << "decelerate" << std::endl;
    _decelerate = keyState;
}

void Player::jump(bool &keyState)
{
    std::cout << "jump" << std::endl;
}

osg::PositionAttitudeTransform *Player::getNode()
{
    return _player;
}

btCollisionObject *Player::getCollisionObject()
{
    return _playerGhostObject;
}

btKinematicCharacterController *Player::getController()
{
    return _playerController;
}