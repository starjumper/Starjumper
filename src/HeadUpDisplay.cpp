#include "HeadUpDisplay.h"

HeadUpDisplay::HeadUpDisplay(Player *player) : 
    _player(player)
{
    _node = new osg::Geode;
    _node->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    
	initializeCamera();
	initializeSpeedBar();
	
	/*
	setUpTimer();
	theTime = new osgText::Text();
    hudGeode->addDrawable(theTime);
	theTime->setFont("fonts/arial.ttf");
	theTime->setPosition(osg::Vec3(50.0f, 50.0f, 0.0f));*/
}

void HeadUpDisplay::initializeCamera()
{
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 640, 0, 480));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	_camera->setRenderOrder(osg::Camera::POST_RENDER);
    _camera->addChild(_node);
}

void HeadUpDisplay::initializeSpeedBar()
{
    _speedBar = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(60, 300, 0), 50, 200, 1));
	_node->addDrawable(_speedBar);
}

void HeadUpDisplay::initializeTimer()
{
    /*
	theTime = new osgText::Text();
    hudGeode->addDrawable(theTime);
	theTime->setFont("fonts/arial.ttf");
	theTime->setPosition(osg::Vec3(50.0f, 50.0f, 0.0f));*/
}

void HeadUpDisplay::update(clock_t start_time, clock_t current_time)
{
//	updateSpeed(player->getSpeed());
	updateSpeed(1.0f);
//	updateTime(start_time, current_time);
}

void HeadUpDisplay::updateSpeed(float playerSpeed)
{
    _speedBar->setColor(osg::Vec4(1.0, 0.3, 0.8, 1.0));
    ((osg::Box *)_speedBar->getShape())->setHalfLengths(osg::Vec3(40, 150, 1));
    /*
	speedDrawable->setColor(osg::Vec4(1.0 * playerSpeed, 0.3, 0.8, 1.0));
	speedBar->setHalfLengths(osg::Vec3(40, 150 * playerSpeed, 1));*/
}

void HeadUpDisplay::updateTime(clock_t start_time, clock_t current_time)
{/*
	std::stringstream ss;
	ss << (current_time - start_time) / 1000;
	std::string time = ss.str();
    if(time.size() > 2)
        time.insert(time.size() - 2, ":", 1);
    theTime->setText("Time elapsed: " + time);*/
}