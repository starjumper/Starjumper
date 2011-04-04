#pragma once

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

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

class FinishUpdater : public osg::NodeCallback 
{
private:
    double _previousStepTime;
public:
    FinishUpdater();    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
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