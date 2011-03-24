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
	initializeSpeedometer();
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

void HeadUpDisplay::initializeSpeedometer()
{
	_speedPat = new osg::PositionAttitudeTransform();
	_hudPat->addChild(_speedPat);
	
	_speedBarMatrixTrans = new osg::MatrixTransform;
	_speedPat->addChild(_speedBarMatrixTrans);

	osg::Node *_speedBarNode = osgDB::readNodeFile(SPEEDBAR_MODEL);
	if(!_speedBarNode)
    {
        throw std::runtime_error("Unable to load player model file!");
    }

	_speedBarPat = new osg::PositionAttitudeTransform();
	_speedBarPat->addChild(_speedBarNode);
	_speedBarPat->setScale(osg::Vec3d(30.0, 10.0, 30.0));
	_speedBarPat->setAttitude(osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3(0.0f, 1.0f , 0.0f)));
	_speedBarMatrixTrans->addChild(_speedBarPat);
	_speedPat->setPosition(SPEEDOMETER_POSITION);
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

void HeadUpDisplay::updateSpeedometer()
{
	float playerSpeed = _player->getPlayerState()->getSpeed();
	//_speedBarPat->setAttitude(osg::Quat(osg::DegreesToRadians(playerSpeed*180.0), 1.0, 0.0, 0.0));
	_speedBarMatrixTrans->setMatrix(osg::Matrix::rotate(osg::inDegrees(-(playerSpeed*270) -135), 0.0f, 0.0f, 1.0f));
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

    hud->updateSpeedometer();
    hud->updateTimer();
}