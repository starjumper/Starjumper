#pragma once

#include <osg/Geode>

#include <btBulletDynamicsCommon.h>

#include "types.h"
#include "Player.h"

class EnvironmentObject : public osg::Geode
{
private:
    btRigidBody *_rigidBody;
       
public:  
    EnvironmentObject();
        
	void setRigidBody(btRigidBody *rigidBody);
    btRigidBody *getRigidBody() const;
    
    virtual void applyTo(Player *player) { };
};
