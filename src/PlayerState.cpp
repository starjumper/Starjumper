#include "PlayerState.h"

PlayerState::PlayerState()
{
    reset();
}

void PlayerState::reset()
{
    _speed = 0.0f;
    _angleX = 0.0f;
    _angleY = 0.0f;
    
    _direction = btVector3(0,0,4);
    
    _requestMoveLeft = false;
    _requestMoveRight = false;
    _requestAccelerate = false;
    _requestDecelerate = false;
    _requestJump = false;
    _dead = false;
}

float PlayerState::getSpeed() const
{
    return _speed;
}

void PlayerState::setSpeed(const float speed)
{
    _speed = speed;
}

float PlayerState::getAngleX() const
{
    return _angleX;
}

void PlayerState::setAngleX(const float angle)
{
    _angleX = angle;
}

float PlayerState::getAngleY() const
{
    return _angleY;
}

void PlayerState::setAngleY(const float angle)
{
    _angleY = angle;
}

btVector3 &PlayerState::getDirection()
{
    return _direction;
}

void PlayerState::setDirection(btVector3 &direction)
{
    _direction = direction;
}

void PlayerState::setRequestMoveLeft(const bool activate)
{
    _requestMoveLeft = activate;
}

void PlayerState::setRequestMoveRight(const bool activate)
{
    _requestMoveRight = activate;
}

void PlayerState::setRequestAccelerate(const bool activate)
{
    _requestAccelerate = activate;
}

void PlayerState::setRequestDecelerate(const bool activate)
{
    _requestDecelerate = activate;
}

void PlayerState::setRequestJump(const bool activate)
{
    _requestJump = activate;
}

bool PlayerState::requestMoveLeft() const
{
    return _requestMoveLeft;
}

bool PlayerState::requestMoveRight() const
{
    return _requestMoveRight;
}

bool PlayerState::requestAccelerate() const
{
    return _requestAccelerate;
}

bool PlayerState::requestDecelerate() const
{
    return _requestDecelerate;
}

bool PlayerState::requestJump() const
{
    return _requestJump;
}

void PlayerState::beDead()
{
    _dead = true;
}

void PlayerState::beAlive()
{
    _dead = false;
}

bool PlayerState::isDead() const
{
    return _dead;
}