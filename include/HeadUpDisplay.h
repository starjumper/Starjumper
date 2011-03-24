#pragma once

#include <sstream>
#include <sys/timeb.h>

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>

#include "Player.h"

#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT       768

#define TIMER_POSITION      osg::Vec3(600, 700, 0)
#define TIMER_FONT          "fonts/arial.ttf"


#define SPEEDBAR_MODEL		"resources/models/speed_bar.osg"
#define SPEEDBG_MODEL		"resources/models/speed_background.osg"
#define SPEEDOMETER_POSITION osg::Vec3(150.0, 150.0, 0.0)

#define SPEEDBAR_POSITION   osg::Vec3(150 ,0 , 0)
#define SPEEDBAR_WIDTH      10.0f
#define SPEEDBAR_LENGTH		200.0f


class HeadUpDisplay : public osg::Referenced
{
private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::Geode *_timeNode;
    osgText::Text *_timer;	

    Player *_player;
    struct timeb _startTime;

	osg::Node *_speedBarNode;
	osg::PositionAttitudeTransform *_hudPat;
	osg::PositionAttitudeTransform *_speedPat;
	osg::PositionAttitudeTransform *_speedBarPat;
	osg::PositionAttitudeTransform *_speedBarBackgroundPat;
	osg::MatrixTransform *_speedBarMatrixTrans;

public:
	HeadUpDisplay(Player *player);
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