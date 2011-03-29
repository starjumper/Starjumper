#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osgText/Text3D>
#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osg/ShapeDrawable>

#include "types.h"
#include "RenderingInstance.h"
#include "Player.h"

#define TIMER_POSITION      osg::Vec3(900, 700, 0)
#define TIMER_FONT          "fonts/arial.ttf"
#define CREDITS_BACKGROUND_COLOR   1.0f, 1.0f, 1.0f, 0.0f

class Credits : public RenderingInstance
{
private:
	void initializeBackground();
	osg::MatrixTransform *_rotate;

public:
	Credits(osgViewer::Viewer *viewer);
	osg::MatrixTransform *getRotate();
	virtual void prepare(osgViewer::Viewer *viewer);
    virtual void cleanup(osgViewer::Viewer *viewer);
};

class CreditsUpdater : public osg::NodeCallback
{
private:
	Credits *_credits;

public:
	CreditsUpdater(Credits *credits);
	virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};