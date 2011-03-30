#include "Player.h"

Player *Player::_instance = NULL;

Player::Player()
{
    loadPlayerModel();
    setScale(PLAYER_SCALE);
    setAttitude(PLAYER_ATTITUDE);
    resetPosition();
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

void Player::resetPosition()
{
    setPosition(PLAYER_HOME_POSITION);    
}
