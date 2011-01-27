#pragma HeadUpDisplay_H__

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osg/ShapeDrawable>

#include "Player.h"

#define SPEED_BAR_POSITION osg::Vec3f(50, 100, 1)
#define SPEED_BAR_SIZE float 20
#define SPEED_BAR_SIZE_X float 50
#define SPEED_BAR_SIZE_Y float 100
#define SPEED_BAR_SIZE_Z float 10

class HeadUpDisplay
{
private:
	osg::Camera *_camera;
	osg::Geode *_node;
	osg::ShapeDrawable *_speedBar;

    Player *_player;
    
public:
	HeadUpDisplay(Player *player);
	osg::Camera *getCamera() {return _camera;}
	void initializeCamera();
	void initializeSpeedBar();
	void initializeTimer();
	void update(clock_t start_time, clock_t current_time);
	void updateSpeed(float playerSpeed);
	void updateTime(clock_t start_time, clock_t current_time);
};