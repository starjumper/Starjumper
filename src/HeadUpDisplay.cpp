#include "HeadUpDisplay.h"
#include <sstream>

extern osgViewer::Viewer viewer;

HeadUpDisplay::HeadUpDisplay() 
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
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, viewer.getCamera()->getViewport()->width(), 0, viewer.getCamera()->getViewport()->height()));
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
        throw std::runtime_error("Unable to load speedbar model file!");
    }

	osg::Node *_speedBarBackgroundNode = osgDB::readNodeFile(SPEEDBG_MODEL);
	if(!_speedBarNode)
    {
        throw std::runtime_error("Unable to load speedbar background model file!");
    }

	osg::StateSet *speedBarBackgroundState = _speedBarBackgroundNode->getOrCreateStateSet();
	osg::Material *material = new osg::Material();
	material->setAlpha(osg::Material::FRONT_AND_BACK, HUD_TRANSPARENCY);
	speedBarBackgroundState->setAttributeAndModes(material, osg::StateAttribute::ON);
	osg::BlendFunc *blendfunc = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
	speedBarBackgroundState->setAttributeAndModes(blendfunc);
	

	_speedBarBackgroundPat = new osg::PositionAttitudeTransform();
	_speedBarBackgroundPat->addChild(_speedBarBackgroundNode);
	_speedBarBackgroundPat->setScale(osg::Vec3d(60.0, 10.0, 60.0));
	//_speedBarBackgroundPat->setPosition(osg::Vec3d(100.0, 100.0, 100.0));
	_speedBarBackgroundPat->setAttitude(osg::Quat(osg::DegreesToRadians(270.0f), osg::Vec3(1.0f, 0.0f , 0.0f)));
	_speedPat->addChild(_speedBarBackgroundPat);

	_speedBarPat = new osg::PositionAttitudeTransform();
	_speedBarPat->addChild(_speedBarNode);
	_speedBarPat->setScale(osg::Vec3d(12.0, 10.0, 12.0));
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
    
    // place timer on top right
	_timer->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - 200, 
	                                viewer.getCamera()->getViewport()->height() - 50, 0));
	
    _timeNode->addDrawable(_timer);
	_hudPat->addChild(_timeNode);
}

void HeadUpDisplay::resetTimer()
{
    ftime(&_startTime);
}

void HeadUpDisplay::updateSpeedometer()
{
	float playerSpeed = Player::getInstance()->getPlayerState()->getSpeed();
	_speedBarMatrixTrans->setMatrix(osg::Matrix::rotate(osg::inDegrees(-(playerSpeed*270) -135), 0.0f, 0.0f, 1.0f));
 }

void HeadUpDisplay::updateTimer()
{
    struct timeb currentTime;

    // get current time
    ftime(&currentTime);
    
    // calculate offset from start time
    time_t _timePassed = (((currentTime.time - _startTime.time) * 1000) + (currentTime.millitm - _startTime.millitm)) / 10;
    
    // extract miliseconds, seconds and minutes
    time_t ms = _timePassed % 100;
    time_t  s = (_timePassed / 100) % 60;
    time_t  m = (_timePassed / 100 / 60) % 60;
    
    // construct time string
	std::stringstream ss;
	ss <<
	    (m  < 10 ? "0" : "") << m << ":" <<
	    (s  < 10 ? "0" : "") << s << ":" <<
        (ms < 10 ? "0" : "") << ms;
	std::string timeString = ss.str();
    
    _timer->setText(timeString);
}

void HeadUpDisplayUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    osg::ref_ptr<HeadUpDisplay> hud = dynamic_cast<HeadUpDisplay *> (node->getUserData());

    hud->updateSpeedometer();
    hud->updateTimer();
}