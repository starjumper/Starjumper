#include "HeadUpDisplay.h"


// _camera -> _hudPat -> _timeNode -> _timer
//		              -> _speedPat -> _speedBarPat -> _speedNode

HeadUpDisplay::HeadUpDisplay(Player *player) : 
    _player(player)
{
	_hudPat = new osg::PositionAttitudeTransform();
	_hudPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	_hudPat->setUserData(this);
	_hudPat->setUpdateCallback(new HeadUpDisplayUpdateCallback);

	initializeCamera();
	initializeTimer();
	initializeSpeedBar();
}

void HeadUpDisplay::initializeCamera()
{
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	_camera->setRenderOrder(osg::Camera::POST_RENDER);
	_camera->addChild(_hudPat);
}

osg::Camera *HeadUpDisplay::getCamera()
{
    return _camera;
}

void HeadUpDisplay::initializeSpeedBar()
{
	_speedPat = new osg::PositionAttitudeTransform();
	_hudPat->addChild(_speedPat);

	_speedBarPat = new osg::PositionAttitudeTransform();
	_speedPat->addChild(_speedBarPat);

	_speedNode = new osg::Geode();
	_speedBar = new osg::ShapeDrawable(new osg::Box(SPEEDBAR_POSITION, 500, 5, 5));
	_speedNode->addDrawable(_speedBar);
	_speedBarPat->addChild(_speedNode);

	_speedBar->setColor(osg::Vec4(1.0, 0.5, 0.8, 0.5));
	_speedPat->setPosition(osg::Vec3d(800.0, 0.0, 0.0));
}

void HeadUpDisplay::initializeTimer()
{
    resetTimer();
    
	_timeNode = new osg::Geode();

	_timer = new osgText::Text();
	_timer->setFont(TIMER_FONT);
	_timer->setPosition(TIMER_POSITION);
	
    _timeNode->addDrawable(_timer);
	_hudPat->addChild(_timeNode);
}

void HeadUpDisplay::resetTimer()
{
    ftime(&_startTime);
}

void HeadUpDisplay::updateSpeedBar()
{
	float playerSpeed = _player->getPlayerState()->getSpeed();

	//_speedBar->setColor(osg::Vec4(1.0, 0.005, 0.8, 0.5));

    //_speedBar->setColor(osg::Vec4(1.0, playerSpeed, 0.0, 0.5));
    //((osg::Box *)_speedBar->getShape())->setHalfLengths(osg::Vec3(SPEEDBAR_WIDTH, SPEEDBAR_MAX_LENGTH * playerSpeed, 1.0f));

	_speedPat->setAttitude(osg::Quat(osg::DegreesToRadians(-80+(playerSpeed*150.0)), 1.0, 0.0, 0.0));
 
	//speedDrawable->setColor(osg::Vec4(1.0 * playerSpeed, 0.3, 0.8, 1.0));
	//_speedBar->set->setHalfLengths(osg::Vec3(40, 150 * playerSpeed, 1));
}

void HeadUpDisplay::updateTimer()
{
    struct timeb currentTime;

    ftime(&currentTime);
    
    time_t _timePassed = (((currentTime.time - _startTime.time) * 1000) + (currentTime.millitm - _startTime.millitm)) / 10;
        
    
	std::stringstream ss;
	ss << _timePassed;
	std::string timeString = ss.str();
	
    if(timeString.size() > 2)
    {
        timeString.insert(timeString.size() - 2, ":", 1);
    }
    else if(timeString.size() == 2)
    {
        timeString = std::string("0:") + timeString;
    }
    else if(timeString.size() == 1)
    {
        timeString = std::string("0:0") + timeString;
    }
    
    _timer->setText(timeString);
}

void HeadUpDisplayUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    osg::ref_ptr<HeadUpDisplay> hud = dynamic_cast<HeadUpDisplay *> (node->getUserData());

    hud->updateSpeedBar();
    hud->updateTimer();
}