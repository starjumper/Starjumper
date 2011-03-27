#include "PlayerUpdater.h"
#include <iostream>

PlayerUpdater::PlayerUpdater()
{
    // nothing more to do here
}

void PlayerUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    Player *player = dynamic_cast<Player *>(node);
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
        player->setEnginesAccelerating(speed);
    }
    else if(playerState->requestDecelerate())
    {
        playerState->setSpeed(speed - 0.04 >= 0 ? speed - 0.04 : 0);
        player->setEnginesDecelerating(speed);
    }
    
    ////////////////////////////////////////
    // special floor attributes / falling //
    ////////////////////////////////////////
    
    if(playerController->frontalHit())
    {
        std::cout << "Frontal hit with speed=" << speed << std::endl;
        if(speed > 0.8f)
        {
            playerState->beDead();
        }
        speed = 0;
        playerState->setSpeed(0);
        player->setEnginesDecelerating(0);
    }
    else
    {
        if(playerController->onGround())
        {
            // player is on the floor, apply special attributes from ground to player
            EnvironmentObject *groundObject = (EnvironmentObject *)playerController->getGroundObject();
            groundObject->applyTo(player);
        }
        else if(!playerController->onGround())
        {        
            // player is falling, kill player if there is no chance to land on ground anymore
            btVector3 position = playerController->getGhostObject()->getWorldTransform().getOrigin();
            int yBucketIndex = (int)(position.y() / 20.0f);
        
            if(yBucketIndex >= player->getDeadlyAltitudes()->size())
                yBucketIndex = player->getDeadlyAltitudes()->size() - 1;

            float minimum = min((*(player->getDeadlyAltitudes()))[yBucketIndex], (*(player->getDeadlyAltitudes()))[yBucketIndex + 1]);
                
            if(position.z() < (minimum - 5.0f))
                playerState->beDead();
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