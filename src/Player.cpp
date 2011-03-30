#include "Player.h"
#include "PlayerUpdater.h"

Player *Player::_instance = NULL;

Player::Player() :
    _playerState(new PlayerState())
{
    loadPlayerModel();
    setScale(PLAYER_SCALE);
    setAttitude(PLAYER_ATTITUDE);
    resetPosition();
    initializePhysics();    
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

void Player::resetPosition()
{
    setPosition(PLAYER_HOME_POSITION);    
}

void Player::setAngles(const float angleX, const float angleY, const float angleZ)
{
    setAttitude(osg::Quat(
        osg::DegreesToRadians(angleX), osg::Vec3(1.0,0.0,0.0),
        osg::DegreesToRadians(angleY), osg::Vec3(0.0,1.0,0.0),
        osg::DegreesToRadians(angleZ), osg::Vec3(0.0,0.0,1.0)));
}
