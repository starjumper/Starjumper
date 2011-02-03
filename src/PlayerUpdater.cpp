#include "PlayerUpdater.h"

PlayerUpdater::PlayerUpdater(Player *player) :
    _player(player)
{
    // nothing more to do here
}

void PlayerUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    /*osg::PositionAttitudeTransform *playerPAT = dynamic_cast<osg::PositionAttitudeTransform *> (node);
  
    if(playerPAT)
    {
        osg::Vec3d position = playerPAT->getPosition();
        
        
        if(_player->_accelerate)
            position.y() += 1;
        if(_player->_moveLeft)
            position.x() -= 0.5;
        if(_player->_moveRight)
            position.x() += 0.5;
        if(_player->_decelerate)
            position.y() -= 1;
        
        playerPAT->setPosition(position);
    }*/
    
    PlayerState *playerState = _player->getPlayerState();
    osg::Vec3 newPosition = updatePhysics(playerState);
    _player->setPosition(newPosition);
  
    traverse(node, nv); 
}

osg::Vec3 PlayerUpdater::updatePhysics(PlayerState *playerState)
{
    btVector3 direction = playerState->getDirection();
    btPairCachingGhostObject *playerGhost = dynamic_cast<btPairCachingGhostObject *>(_player->getCollisionObject());
    btKinematicCharacterController *playerController = _player->getController();
    
    if(playerState->requestMoveLeft())
        direction -= btVector3(1, 0, 0);
    
    if(playerState->requestMoveRight())
        direction += btVector3(1, 0, 0);
    
    if(playerState->requestAccelerate())
    {
        float speed = playerState->getSpeed();
        
        direction += btVector3(0, 1, 0);
    }
        
    if(playerState->requestDecelerate())
        direction -= btVector3(0, 1, 0);
    
    playerController->setWalkDirection(direction);
    
    if(playerState->requestJump())
    {
        playerState->setRequestJump(false);
        playerController->jump();
    }
    
    playerState->setDirection(direction);
    
    return osgbBullet::asOsgVec3(direction);
}