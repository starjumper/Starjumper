#pragma once

#include <osg/Group>

#include <btBulletDynamicsCommon.h>

class PlayerState
{
private:
    float _speed;
    
    btVector3 _direction;
    
    bool _requestMoveLeft;
    bool _requestMoveRight;
    bool _requestAccelerate;
    bool _requestDecelerate;
    bool _requestJump;
    
public:
    PlayerState();

    float getSpeed();
    void setSpeed(float speed);
    
    btVector3 &getDirection();
    void setDirection(btVector3 &direction);

    void setRequestMoveLeft(bool activate);
    void setRequestMoveRight(bool activate);
    void setRequestAccelerate(bool activate);
    void setRequestDecelerate(bool activate);
    void setRequestJump(bool activate);
    
    bool requestMoveLeft();
    bool requestMoveRight();
    bool requestAccelerate();
    bool requestDecelerate();
    bool requestJump();
};