#include "Player.h"

Player::Player(btDynamicsWorld *world) :
    _world(world)
{
    _player = new osg::PositionAttitudeTransform;
    _playerGhostObject = new btGhostObject;
    
    initializePlayerModel();
    initializePlayerPhysics();
}

void Player::initializePlayerModel()
{
    osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);
    if(!playerModel)
    {
        throw std::runtime_error("Unable to load player model file!");
    }
    
    osg::PositionAttitudeTransform *patPlayer = new osg::PositionAttitudeTransform;   

    patPlayer->addChild(playerModel);
    patPlayer->setScale(PLAYER_SCALE);
    patPlayer->setAttitude(PLAYER_ATTITUDE);

    _player->addChild(patPlayer);
}

void Player::initializePlayerPhysics()
{
    // use a btBoxShape as collision shape for the player
    btBoxShape *boundingBox = new btBoxShape(PLAYER_BBOX_EXTENTS);
    _playerGhostObject->setCollisionShape(boundingBox);
    
    btTransform playerTransform;
	playerTransform.setIdentity ();
	playerTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	_playerGhostObject->setWorldTransform(playerTransform);
}

osg::PositionAttitudeTransform *Player::getPlayer()
{
    return _player;
}

btGhostObject *Player::getPlayerGhostObject()
{
    return _playerGhostObject;
}