#pragma once

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include <btBulletDynamicsCommon.h>
#include <osgbBullet/Utils.h>
#include <osgbBullet/CollisionShapes.h>

#include "CollisionObject.h"

#define FINISH_MODEL_FILE           "resources/models/finish.osg"
#define TUNNEL_MODEL_FILE			"resources/models/tunnel.osg"
#define CUBOID_TUNNEL_MODEL_FILE	"resources/models/tunnel_cuboid.osg"

class CollisionModel : public CollisionObject
{
protected:
	osg::PositionAttitudeTransform *_transform;
		        
public:  
	CollisionModel();
	
    osg::PositionAttitudeTransform *getNode() { return _transform; }
	btRigidBody *getRigidBody();
    
	virtual void collide() = 0;
};

class Finish : public CollisionModel
{
public:
    Finish(osg::Vec3 position);
    void collide() { }
};

class Tunnel : public CollisionModel
{
public:
    Tunnel(osg::Vec3 position, float length);
    void collide() { }
};

class CuboidTunnel : public CollisionModel
{
public:
    CuboidTunnel(osg::Vec3 position, float length);
    void collide() { }
};