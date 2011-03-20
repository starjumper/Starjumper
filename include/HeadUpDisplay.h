#pragma once

#include <sstream>
#include <sys/timeb.h>

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osg/ShapeDrawable>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>

#include "Player.h"

#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT       768

#define TIMER_POSITION      osg::Vec3(900, 700, 0)
#define TIMER_FONT          "fonts/arial.ttf"


//#define SPEEDBAR_POSITION   osg::Vec3f(60, SCREEN_HEIGHT / 2.0f, 0)
#define SPEEDBAR_POSITION   osg::Vec3(500,500, 0)
#define SPEEDBAR_WIDTH      20.0f
#define SPEEDBAR_MAX_LENGTH 200.0f


class HeadUpDisplay : public osg::Referenced
{
private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::Geode *_timeNode;
	osg::ShapeDrawable *_speedBar;
    osgText::Text *_timer;

    Player *_player;
    struct timeb _startTime;

	//
	//
	//

	osg::Geode *_speedNode;

	osg::PositionAttitudeTransform *_hudPat;
	osg::PositionAttitudeTransform *_speedPat;

	osg::PositionAttitudeTransform *_speedBarPat;

public:
	HeadUpDisplay(Player *player);
    osg::Camera *getCamera();

	void initializeCamera();
	void initializeSpeedBar();
	void initializeTimer();
	
    void resetTimer();
	void updateSpeedBar();
	void updateTimer();
};

class HeadUpDisplayUpdateCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};