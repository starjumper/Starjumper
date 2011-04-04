#include "CollisionModel.h"

extern osgViewer::Viewer viewer;

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


    osg::StateSet* stateSet = new osg::StateSet();
    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON); 
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

    tunnelModel->setStateSet(stateSet);
}

FinishUpdater::FinishUpdater() :
    _previousStepTime(0.0f)
{
}

void FinishUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    double currentStepTime = viewer.getFrameStamp()->getSimulationTime();
    
    if(_previousStepTime > 0.0f)
    {
        osg::PositionAttitudeTransform *finish = dynamic_cast<osg::PositionAttitudeTransform *>(node);
    
        osg::Quat attitude = finish->getAttitude();
        osg::Quat::value_type angle;
        osg::Vec3 axis;

        attitude.getRotate(angle, axis);

        float degrees = osg::RadiansToDegrees(angle) + ((currentStepTime-_previousStepTime) * 50.0f);
        degrees = (degrees > 360) ? degrees - 360 : degrees;
    
        finish->setAttitude(osg::Quat(osg::DegreesToRadians(degrees), axis));
    }
    
    _previousStepTime = currentStepTime;
}

Tunnel::Tunnel(osg::Vec3 position, float length) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(TUNNEL_MODEL_FILE);
	
	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
	
	
	osg::StateSet *stateset = new osg::StateSet();
    
    osg::ref_ptr<osg::Material> matirial = new osg::Material;
    matirial->setColorMode(osg::Material::AMBIENT);
    matirial->setAlpha(osg::Material::FRONT_AND_BACK, 0.9);
    matirial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
    matirial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
    matirial->setShininess(osg::Material::FRONT_AND_BACK, 64.0f);
    stateset->setAttributeAndModes(matirial, osg::StateAttribute::ON);
    tunnelModel->setStateSet(stateset);
}

CuboidTunnel::CuboidTunnel(osg::Vec3 position, float length) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(CUBOID_TUNNEL_MODEL_FILE);

	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
}