#pragma once

#include <osg/Geode>

#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "types.h"
#include "EnvironmentObject.h"
#include "Player.h"

#define TUNNEL_MODEL_FILE			"resources/models/tunnel.osg"
#define CUBOID_TUNNEL_MODEL_FILE	"resources/models/tunnel_cuboid.osg"

class Tunnel : public EnvironmentObject
{
private:
	osg::PositionAttitudeTransform *_transform;
	
protected:
	void constructRigidBody();
	        
public:  
	Tunnel(osg::Vec3 position, float length);
	 
    virtual void applyTo(Player *player) { };

	osg::PositionAttitudeTransform *getNode();
};

class CuboidTunnel : public EnvironmentObject
{
private:
	osg::PositionAttitudeTransform *_transform;
	
protected:
	void constructRigidBody();
	        
public:  
	CuboidTunnel(osg::Vec3 position, float length);
	 
    virtual void applyTo(Player *player) { };

	osg::PositionAttitudeTransform *getNode();
};
