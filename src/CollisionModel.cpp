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
	
	
	
    	osg::StateSet *speedBarBackgroundState = tunnelModel->getOrCreateStateSet();
    /*	osg::Material *material = new osg::Material();
    	material->setAlpha(osg::Material::FRONT_AND_BACK, HUD_TRANSPARENCY);
    	speedBarBackgroundState->setAttributeAndModes(material, osg::StateAttribute::ON);
    	osg::BlendFunc *blendfunc = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
    	speedBarBackgroundState->setAttributeAndModes(blendfunc);
    */
/*

        osg::BlendFunc *blendFunc = new osg::BlendFunc();
        osg::BlendColor  *blendColor = new osg::BlendColor(osg::Vec4(1, 0, 1, 1));

         blendFunc->setSource(osg::BlendFunc::CONSTANT_ALPHA);

         blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
         speedBarBackgroundState->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);
         speedBarBackgroundState->setAttributeAndModes(blendColor, osg::StateAttribute::ON);    


        osg::ref_ptr<osg::Material> matirial = new osg::Material;
          matirial->setColorMode(osg::Material::AMBIENT);
          matirial->setAlpha(osg::Material::FRONT_AND_BACK, 0.5);
          matirial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
          matirial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
          matirial->setShininess(osg::Material::FRONT_AND_BACK, 64.0f);
      	  speedBarBackgroundState->setAttributeAndModes(matirial, osg::StateAttribute::ON);*/

}

CuboidTunnel::CuboidTunnel(osg::Vec3 position, float length) :
    CollisionModel()
{
	osg::Node *tunnelModel = osgDB::readNodeFile(CUBOID_TUNNEL_MODEL_FILE);

	_transform->addChild(tunnelModel);
	_transform->setPosition(position);
	_transform->setScale(osg::Vec3f(2.0f, length, 2.0f));
}