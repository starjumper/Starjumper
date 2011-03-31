#pragma once

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osg/Drawable>
#include <osg/Texture2D>

#include "Player.h"

#define BACKGROUND_IMAGE   "resources/textures/space.jpg"

class Sky : public osg::Referenced
{
private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::PositionAttitudeTransform *_skyPat;

public:
	Sky();
    osg::Camera *getCamera();

	void initializeCamera();
};
