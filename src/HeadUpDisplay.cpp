#include "HeadUpDisplay.h"

HeadUpDisplay::HeadUpDisplay(Player *player) : 
    _player(player)
{
    //_node = new osg::Geode;
    //_node->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    
    //_node->setUserData(this);
    //_node->setUpdateCallback(new HeadUpDisplayUpdateCallback);
    
	//initializeCamera();
	//initializeSpeedBar();
    //initializeTimer();

	_hudPat = new osg::PositionAttitudeTransform();
	_hudPat->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	_hudPat->setUserData(this);
	_hudPat->setUpdateCallback(new HeadUpDisplayUpdateCallback);

	initializeCamera();
}

void HeadUpDisplay::initializeCamera()
{
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	_camera->setRenderOrder(osg::Camera::POST_RENDER);
    //_camera->addChild(__node);
	_camera->addChild(_hudPat);
}

osg::Camera *HeadUpDisplay::getCamera()
{
    return _camera;
}

void HeadUpDisplay::initializeSpeedBar()
{
    //_speedBar = new osg::ShapeDrawable(new osg::Box(SPEEDBAR_POSITION, SPEEDBAR_WIDTH, 0, 0));
	//_node->addDrawable(_speedBar);

	_speedPat = new osg::PositionAttitudeTransform();

	_hudPat->addChild(speedPat);
}

void HeadUpDisplay::initializeSpeedMeter()
{
}

void HeadUpDisplay::initializeTimer()
{
    resetTimer();
    
	_timer = new osgText::Text();
	_timer->setFont(TIMER_FONT);
	_timer->setPosition(TIMER_POSITION);
	
    //_node->addDrawable(_timer);
}

void HeadUpDisplay::resetTimer()
{
    _startTime = clock();
    _timePassed = 0;
}

void HeadUpDisplay::updateSpeedBar()
{
	float playerSpeed = _player->getPlayerState()->getSpeed();

//    _speedBar->setColor(osg::Vec4(1.0, 0.005, 0.8, 0.5));

    //_speedBar->setColor(osg::Vec4(1.0, playerSpeed, 0.0, 0.5));
    //((osg::Box *)_speedBar->getShape())->setHalfLengths(osg::Vec3(SPEEDBAR_WIDTH, SPEEDBAR_MAX_LENGTH * playerSpeed, 1.0f));

    /*
	speedDrawable->setColor(osg::Vec4(1.0 * playerSpeed, 0.3, 0.8, 1.0));
	speedBar->setHalfLengths(osg::Vec3(40, 150 * playerSpeed, 1));*/
}

void HeadUpDisplay::updateTimer()
{
    _timePassed = (clock() - _startTime) / 1000;
    
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

    //hud->updateSpeedBar();
    //hud->updateTimer();
}