#include "Goal.h"

Goal::Goal(osg::Vec3 position)
{
	osg::Node *tunnelModel = osgDB::readNodeFile(GOAL_MODEL_FILE);
	_transform = new osg::PositionAttitudeTransform();

	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, 2.0f, 2.0f));
	
	_transform->setNodeMask(RECEIVE_SHADOW_MASK);
	
	constructRigidBody();
}

void Goal::constructRigidBody()
{
	btCollisionShape *mesh = osgbBullet::btTriMeshCollisionShapeFromOSG(_transform);

    btDefaultMotionState *msGoal = new btDefaultMotionState();
    
    btRigidBody::btRigidBodyConstructionInfo rbciGoal(0, msGoal, mesh, btVector3(0,0,0));

	setRigidBody(new btRigidBody(rbciGoal));
	getRigidBody()->setUserPointer(this);
}

osg::PositionAttitudeTransform *Goal::getNode()
{
	return _transform;
}

void Goal::applyTo(Player *player)
{
    player->setReachedFinish(true);
}
