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
    _transform->setAttitude(osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,1.0,0.0)));
    
	FinishUpdater *finishUpdater = new FinishUpdater();
    _transform->setUpdateCallback(finishUpdater);
}

FinishUpdater::FinishUpdater()
{
}

void FinishUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    osg::PositionAttitudeTransform *finish = dynamic_cast<osg::PositionAttitudeTransform *>(node);
    
    osg::Quat attitude = finish->getAttitude();
    osg::Quat::value_type angle;
    osg::Vec3 axis;

    attitude.getRotate(angle, axis);
    
    float degrees = osg::RadiansToDegrees(angle) + 1;
    degrees = (degrees > 360) ? degrees - 360 : degrees;
    
    finish->setAttitude(osg::Quat(osg::DegreesToRadians(degrees), axis));
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