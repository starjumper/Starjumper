#pragma once

#include <osg/Group>

#include <btBulletDynamicsCommon.h>

class PlayerState
{
private:
    float _speed;
    float _angleY;
    
    btVector3 _direction;
    
    bool _requestMoveLeft;
    bool _requestMoveRight;
    bool _requestAccelerate;
    bool _requestDecelerate;
    bool _requestJump;
    
public:
    PlayerState();

    float getSpeed() const;
    void setSpeed(const float speed);
    
    float getAngleY() const;
    void setAngleY(const float angle);
    
    btVector3 &getDirection();
    void setDirection(btVector3 &direction);

    void setRequestMoveLeft(const bool activate);
    void setRequestMoveRight(const bool activate);
    void setRequestAccelerate(const bool activate);
    void setRequestDecelerate(const bool activate);
    void setRequestJump(const bool activate);
    
    bool requestMoveLeft() const;
    bool requestMoveRight() const;
    bool requestAccelerate() const;
    bool requestDecelerate() const;
    bool requestJump() const;
};