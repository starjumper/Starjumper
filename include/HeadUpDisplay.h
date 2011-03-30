#pragma once

#include <sys/timeb.h>

#include <osg/BlendFunc>
#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>
#include <osgViewer/Viewer>

#include "Player.h"

#define TIMER_FONT          "fonts/arial.ttf"

#define SPEEDBAR_MODEL		"resources/models/speed_bar.osg"
#define SPEEDBG_MODEL		"resources/models/speed_background.osg"
#define SPEEDOMETER_POSITION osg::Vec3(150.0, 150.0, 0.0)

#define SPEEDBAR_POSITION   osg::Vec3(150 ,0 , 0)
#define SPEEDBAR_WIDTH      10.0f
#define SPEEDBAR_LENGTH		200.0f

#define HUD_TRANSPARENCY	0.2


class HeadUpDisplay : public osg::Referenced
{
private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::Geode *_timeNode;
    osgText::Text *_timer;	

    struct timeb _startTime;

	osg::Node *_speedBarNode;
	osg::PositionAttitudeTransform *_hudPat;
	osg::PositionAttitudeTransform *_speedPat;
	osg::PositionAttitudeTransform *_speedBarPat;
	osg::PositionAttitudeTransform *_speedBarBackgroundPat;
	osg::MatrixTransform *_speedBarMatrixTrans;

public:
	HeadUpDisplay();
    osg::Camera *getCamera();

	void initializeCamera();
	void initializeSpeedometer();
	void initializeTimer();
	
    void resetTimer();
	void updateSpeedometer();
	void updateTimer();
};

class HeadUpDisplayUpdateCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};