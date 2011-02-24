#include "Lighting.h"

Lighting::Lighting() 
{
	_light = new osg::Light;
	_light->setLightNum(1);
	_light->setSpecular(osg::Vec4d(1.0, 1.0, 1.0, 1.0));
	_light->setAmbient(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	_light->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	_light->setConstantAttenuation(1.0f);
	_light->setPosition(osg::Vec4d(0.0, 0.0, 1.5, 1.0));
}

osg::Node *Lighting::getLights(osg::StateSet *rootStateSet) 
{
	osg::Group *lightGroup = new osg::Group;	
	osg::LightSource *lightSource = new osg::LightSource;   
	lightSource->setLight(_light);
	osg::StateSet *stateset = lightGroup->getOrCreateStateSet();
	lightSource->setStateSetModes(*stateset, osg::StateAttribute::ON);
	lightSource->setStateSetModes(*rootStateSet,osg::StateAttribute::ON);
   
	lightGroup->addChild(lightSource);

	return lightGroup;
}

void Lighting::setLightPosition(osg::Vec3 position)
{
	_light->setPosition(osg::Vec4(position, 1.0));
}