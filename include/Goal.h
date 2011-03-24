#pragma once

#include <osg/Geode>

#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "types.h"
#include "EnvironmentObject.h"
#include "Player.h"

#define GOAL_MODEL_FILE			"resources/models/goal.osg"

class Goal : public EnvironmentObject
{
private:
	osg::PositionAttitudeTransform *_transform;
	
protected:
	void constructRigidBody();
	        
public:  
	Goal(osg::Vec3 position);
	
	osg::PositionAttitudeTransform *getNode();
	
	virtual void applyTo(Player *player);
};
