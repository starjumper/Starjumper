#include "Tunnel.h"

Tunnel::Tunnel(osg::Vec3 position, float length)
{
	osg::Node *tunnelModel = osgDB::readNodeFile(TUNNEL_MODEL_FILE);
	_transform = new osg::PositionAttitudeTransform();

	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
	
	_transform->setNodeMask(RECEIVE_SHADOW_MASK);
	
	constructRigidBody();
}

void Tunnel::constructRigidBody()
{
	btCollisionShape *mesh = osgbBullet::btTriMeshCollisionShapeFromOSG(_transform);

    btDefaultMotionState *msTunnel = new btDefaultMotionState();
    
    btRigidBody::btRigidBodyConstructionInfo rbciTunnel(0, msTunnel, mesh, btVector3(0,0,0));

	setRigidBody(new btRigidBody(rbciTunnel));
	getRigidBody()->setUserPointer(this);
}

osg::PositionAttitudeTransform *Tunnel::getNode()
{
	return _transform;
}

CuboidTunnel::CuboidTunnel(osg::Vec3 position, float length)
{
	osg::Node *tunnelModel = osgDB::readNodeFile(CUBOID_TUNNEL_MODEL_FILE);
	_transform = new osg::PositionAttitudeTransform();

	getNode()->addChild(tunnelModel);
	getNode()->setPosition(position);
	getNode()->setScale(osg::Vec3f(2.0f, length, 2.0f));
	
	getNode()->setNodeMask(RECEIVE_SHADOW_MASK);
	
	constructRigidBody();
}

void CuboidTunnel::constructRigidBody()
{
	btCollisionShape *mesh = osgbBullet::btTriMeshCollisionShapeFromOSG(_transform);

    btDefaultMotionState *msTunnel = new btDefaultMotionState();
    
    btRigidBody::btRigidBodyConstructionInfo rbciTunnel(0, msTunnel, mesh, btVector3(0,0,0));

	setRigidBody(new btRigidBody(rbciTunnel));
	getRigidBody()->setUserPointer(this);
}

osg::PositionAttitudeTransform *CuboidTunnel::getNode()
{
	return _transform;
}