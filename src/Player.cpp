#include "Player.h"

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
	patPlayer->setPosition(osg::Vec3(0.0,0.0,5.0));

    _player->addChild(patPlayer);
	_player->setNodeMask(CAST_SHADOW_MASK);
}

void Player::initializePlayerPhysics()
{
    _playerGhostObject = new btPairCachingGhostObject;
    
    // use a btBoxShape as collision shape for the player
    btBoxShape *boundingBox = new btBoxShape(PLAYER_BBOX_EXTENTS);
    _playerGhostObject->setCollisionShape(boundingBox);
    _playerGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    
    btTransform playerTransform;
	playerTransform.setIdentity ();
	playerTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	_playerGhostObject->setWorldTransform(playerTransform);
	
	_playerController = new btKinematicCharacterController(_playerGhostObject, boundingBox, btScalar(0.35));
}

void Player::moveLeft(bool &keyState)
{
    std::cout << "moveLeft" << std::endl;
}

void Player::moveRight(bool &keyState)
{
    std::cout << "moveRight" << std::endl;
}

void Player::accelerate(bool &keyState)
{
    std::cout << "accelerate" << std::endl;
}

void Player::decelerate(bool &keyState)
{
    std::cout << "decelerate" << std::endl;
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