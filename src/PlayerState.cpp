#include "PlayerState.h"

PlayerState::PlayerState()
{
    // initialize state variables
    _speed = 0.0f;
    
    _direction = &btVector3(0,0,4);
    
    _requestMoveLeft = false;
    _requestMoveRight = false;
    _requestAccelerate = false;
    _requestDecelerate = false;
    _requestJump = false;
}

float PlayerState::getSpeed()
{
    return _speed;
}

void PlayerState::setSpeed(float speed)
{
    _speed = speed;
}

btVector3 PlayerState::getDirection()
{
    return *_direction;
}

void PlayerState::setDirection(btVector3 *direction)
{
    _direction = direction;
}

void PlayerState::setRequestMoveLeft(bool activate)
{
    _requestMoveLeft = activate;
}

void PlayerState::setRequestMoveRight(bool activate)
{
    _requestMoveRight = activate;
}

void PlayerState::setRequestAccelerate(bool activate)
{
    _requestAccelerate = activate;
}

void PlayerState::setRequestDecelerate(bool activate)
{
    _requestDecelerate = activate;
}

void PlayerState::setRequestJump(bool activate)
{
    _requestJump = activate;
}

bool PlayerState::requestMoveLeft()
{
    return _requestMoveLeft;
}

bool PlayerState::requestMoveRight()
{
    return _requestMoveRight;
}

bool PlayerState::requestAccelerate()
{
    return _requestAccelerate;
}

bool PlayerState::requestDecelerate()
{
    return _requestDecelerate;
}

bool PlayerState::requestJump()
{
    return _requestJump;
}