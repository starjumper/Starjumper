#include "PlayerUpdater.h"

PlayerUpdater::PlayerUpdater(Player *player) :
    _player(player)
{
    // nothing more to do here
}

void PlayerUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    PlayerState *playerState = _player->getPlayerState();
    osg::Vec3 newPosition = calculateNextPosition(playerState);
    _player->setPosition(newPosition);
    //_player->setAngles(playerState->getAngleX(), playerState->getAngleY());
    _player->setAngles(0, playerState->getAngleY());
    traverse(node, nv); 
}

osg::Vec3 PlayerUpdater::calculateNextPosition(PlayerState *playerState)
{
    KinematicCharacterController *playerController = _player->getController();
    float speed = playerState->getSpeed();
    float angleY = playerState->getAngleY();
    float angleX = playerState->getAngleX();
    
    btVector3 direction = btVector3(0, 0, 0);
    
    // check for move requests
    
    if(playerState->requestMoveLeft())
    {
        direction -= btVector3(speed / 10.0f, 0, 0);
        playerState->setAngleY(angleY + 3 < 30 ? angleY + 3 : 30);
    }
    else if(playerState->requestMoveRight())
    {
        direction += btVector3(speed / 10.0f, 0, 0);
        playerState->setAngleY(angleY - 3 > -30 ? angleY - 3 : -30);
    }
    else
    {
        if(angleY > 0)
            playerState->setAngleY(angleY - 5 > 0 ? angleY - 5 : 0);
        else if(angleY < 0)
            playerState->setAngleY(angleY + 5 < 0 ? angleY + 5 : 0);
        else
            playerState->setAngleY(0);
    }
    
    if(playerState->requestAccelerate())
    {
        playerState->setSpeed(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0);
        playerState->setAngleX(angleX - 3 > -10 ? angleX - 3 : -10);
        _player->setEnginesAccelerating(speed);
    }
    else if(playerState->requestDecelerate())
    {
        playerState->setSpeed(speed - 0.04 >= 0 ? speed - 0.04 : 0);
        playerState->setAngleX(angleX + 3 < 10 ? angleX + 3 : 10);
        _player->setEnginesDecelerating(speed);
    }
    else
    {
        if(angleX > 0)
            playerState->setAngleX(angleX - 1 > 0 ? angleX - 1 : 0);
        else if(angleX < 0)
            playerState->setAngleX(angleX + 1 < 0 ? angleX + 1 : 0);
        else
            playerState->setAngleX(0);
    }
    
    if(playerController->onGround())
    {
        // apply special attributes from ground to player
//        Cuboid *groundObject = (Cuboid *)playerController->getGroundObject();
 //       groundObject->applyTo(_player);
    }
    else
    {
        // check whether player is falling and whether there is no more possibility
        // to fall on a block

        btVector3 position = playerController->getGhostObject()->getWorldTransform().getOrigin();
        int yBucketIndex = (int)(position.y() / 20.0f);
        float minimum = min((*(_player->getDeadlyAltitudes()))[yBucketIndex], (*(_player->getDeadlyAltitudes()))[yBucketIndex + 1]);
                
        if(position.z() < (minimum - 5.0f))
            playerState->beDead();
    }
    
    direction += btVector3(0, playerState->getSpeed(), 0);

    if(playerState->requestJump())
        playerController->jump();
    
    playerController->setWalkDirection(direction);
    
    btVector3 position = playerController->getGhostObject()->getWorldTransform().getOrigin();
    
    return osgbBullet::asOsgVec3(position);
}