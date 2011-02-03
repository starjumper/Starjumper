#include "Lighting.h"

Lighting::Lighting() 
{
	
}

osg::Node* Lighting::getLights(osg::StateSet* rootStateSet) 
{
	osg::Group* lightGroup = new osg::Group;

	osg::Light* light = new osg::Light;
	light->setLightNum(1);
	light->setSpecular(osg::Vec4d(1.0, 1.0, 1.0, 1.0));
	//light->setAmbient(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	//light->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	//light->setConstantAttenuation(1.0f);
	light->setPosition(osg::Vec4d(0.0, 0.0, 1.5, 1.0));

	osg::LightSource* lightSource = new osg::LightSource;   
	lightSource->setLight(light);
	osg::StateSet * stateset = lightGroup->getOrCreateStateSet();
	lightSource->setStateSetModes(*stateset, osg::StateAttribute::ON);
	lightSource->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);

	osg::MatrixTransform* mt = new osg::MatrixTransform();
	{
		// set up the animation path
		osg::AnimationPath* animationPath = new osg::AnimationPath;
		animationPath->insert(0.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,0.0)));
		animationPath->insert(1.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,2.0)));
		animationPath->insert(2.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,3.0)));
		animationPath->insert(3.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,4.0)));
		animationPath->insert(4.0,osg::AnimationPath::ControlPoint(osg::Vec3(0.0,0.0,4.0)));
		animationPath->insert(5.0,osg::AnimationPath::ControlPoint(osg::Vec3(2.0,0.0,4.0)));
		animationPath->insert(6.0,osg::AnimationPath::ControlPoint(osg::Vec3(2.0,0.0,3.0)));
		animationPath->insert(7.0,osg::AnimationPath::ControlPoint(osg::Vec3(2.0,0.0,2.0)));
		animationPath->insert(8.0,osg::AnimationPath::ControlPoint(osg::Vec3(2.0,0.0,0.0)));
		animationPath->insert(9.0,osg::AnimationPath::ControlPoint(osg::Vec3(-2.0,0.0,0.0)));
		animationPath->setLoopMode(osg::AnimationPath::SWING);

		mt->setUpdateCallback(new osg::AnimationPathCallback(animationPath));
	}
	
	mt->addChild(lightSource);
   
	lightGroup->addChild(mt);

	return lightGroup;
}