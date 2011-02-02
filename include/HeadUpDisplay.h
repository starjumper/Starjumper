#pragma once

#include <sstream>

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

#define SPEEDBAR_POSITION   osg::Vec3f(60, SCREEN_HEIGHT / 2.0f, 0)
#define SPEEDBAR_WIDTH      20.0f
#define SPEEDBAR_MAX_LENGTH 400.0f


class HeadUpDisplay : public osg::Referenced
{
private:
	osg::Camera *_camera;
	osg::Geode *_node;
	osg::ShapeDrawable *_speedBar;
    osgText::Text *_timer;

    Player *_player;
    clock_t _startTime;

public:
	HeadUpDisplay(Player *player);
    osg::Camera *getCamera();

	void initializeCamera();
	void initializeSpeedBar();
	void initializeTimer();
	
	void updateSpeedBar();
	void updateTimer();
};

class HeadUpDisplayUpdateCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};