#include "PlayerUpdater.h"
#include <iostream>

PlayerUpdater::PlayerUpdater()
{
    // nothing more to do here    
}

void PlayerUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    Player *player = Player::getInstance();

    if(player)
    {
        osg::Vec3 newPosition = calculateNextPosition(player);
        player->setPosition(newPosition);
        player->setAngles(0, player->getPlayerState()->getAngleY());
    }    
            
    traverse(node, nv);
}

osg::Vec3 PlayerUpdater::calculateNextPosition(Player *player)
{
    PlayerState *playerState = player->getPlayerState();
    KinematicCharacterController *playerController = player->getController();
    
    float speed = playerState->getSpeed();
    float angleY = playerState->getAngleY();
    
    btVector3 direction = btVector3(0, 0, 0);
    
    // check for move requests
    
    ///////////////////////
    // sideways movement //
    ///////////////////////
    
        
    if(playerState->requestMoveLeft())
    {
        direction -= btVector3(speed / 10.0f, 0, 0);
        playerState->setAngleY(angleY + 3 < 30 ? angleY + 3 : 30);
        playerState->setDirectionX(-1);
    }
    else if(playerState->requestMoveRight())
    {
        direction += btVector3(speed / 10.0f, 0, 0);
        playerState->setAngleY(angleY - 3 > -30 ? angleY - 3 : -30);
        playerState->setDirectionX(1);
    }
    else
    {
        if(angleY > 0)
            playerState->setAngleY(angleY - 5 > 0 ? angleY - 5 : 0);
        else if(angleY < 0)
            playerState->setAngleY(angleY + 5 < 0 ? angleY + 5 : 0);
        else
            playerState->setAngleY(0);
        playerState->setDirectionX(0);
    }
    
    /////////////////////////////////
    // acceleration / deceleration //
    /////////////////////////////////
    
    if(playerState->requestAccelerate())
    {
        playerState->setSpeed(speed + 0.02 <= 1.0 ? speed + 0.02 : 1.0);
        player->setEngines(speed, ACCELERATE);
    }
    else if(playerState->requestDecelerate())
    {
        playerState->setSpeed(speed - 0.04 >= 0 ? speed - 0.04 : 0);
        player->setEngines(speed, DECELERATE);
    }
    
    ////////////////////////////////////////
    // special floor attributes / falling //
    ////////////////////////////////////////
    
    if(playerController->frontalHit())
    {
        if(speed > 0.8f)
        {
            playerState->beDead();
        }
        speed = 0;
        playerState->setSpeed(0);
        player->setEngines(0, DECELERATE);
    }
    else
    {
        if(playerController->onGround())
        {
            // player is on the floor, apply special attributes from ground to player
            CollisionObject *groundObject = (CollisionObject *)playerController->getGroundObject();
            groundObject->collide();
        }
    }
    
    direction += btVector3(0, playerState->getSpeed(), 0);
    playerController->setWalkDirection(direction);    

    // handle jump request
    if(playerState->requestJump())
        playerController->jump();
    
    btVector3 position = playerController->getGhostObject()->getWorldTransform().getOrigin();
        
    return osgbBullet::asOsgVec3(position);
}

