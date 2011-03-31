#include "CollisionModel.h"

CollisionModel::CollisionModel()
{
    _transform = new osg::PositionAttitudeTransform();
	_transform->setNodeMask(RECEIVE_SHADOW_MASK);	
}

btRigidBody *CollisionModel::getRigidBody()
{
	btCollisionShape *mesh = osgbBullet::btTriMeshCollisionShapeFromOSG(_transform);
    btDefaultMotionState *msGoal = new btDefaultMotionState();
    
    btRigidBody::btRigidBodyConstructionInfo rbciGoal(0, msGoal, mesh, btVector3(0, 0, 0));

	btRigidBody *rigidBody = new btRigidBody(rbciGoal);
	rigidBody->setUserPointer(this);

    return rigidBody;
}

Finish::Finish(osg::Vec3 position) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(FINISH_MODEL_FILE);
	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, 2.0f, 2.0f));
}

Tunnel::Tunnel(osg::Vec3 position, float length) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(TUNNEL_MODEL_FILE);
	
	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
}

CuboidTunnel::CuboidTunnel(osg::Vec3 position, float length) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(CUBOID_TUNNEL_MODEL_FILE);

	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
}