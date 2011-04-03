#include "HeadUpDisplay.h"
#include <sstream>

extern osgViewer::Viewer viewer;

HeadUpDisplay::HeadUpDisplay() :
    _isTiming(false)
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
        throw std::runtime_error("Unable to load speedbar model file!");

	osg::Node *_speedBarBackgroundNode = osgDB::readNodeFile(SPEEDBG_MODEL);
	if(!_speedBarNode)
        throw std::runtime_error("Unable to load speedbar background model file!");

	osg::StateSet *speedBarBackgroundState = new osg::StateSet();
	osg::Material *material = new osg::Material();
	material->setAlpha(osg::Material::FRONT_AND_BACK, HUD_TRANSPARENCY);
	speedBarBackgroundState->setAttributeAndModes(material, osg::StateAttribute::ON);
	osg::BlendFunc *blendfunc = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
    speedBarBackgroundState->setAttributeAndModes(blendfunc);      

	osg::Texture2D *texture = new osg::Texture2D;	
	osg::Image *image = osgDB::readImageFile(HUD_TEXTURE);
	texture->setImage(image);

	speedBarBackgroundState->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	_speedBarBackgroundNode->setStateSet(speedBarBackgroundState);

    int speedBarSize = viewer.getCamera()->getViewport()->height() / 12.0f;
    
	_speedBarBackgroundPat = new osg::PositionAttitudeTransform();
	_speedBarBackgroundPat->addChild(_speedBarBackgroundNode);
	_speedBarBackgroundPat->setScale(osg::Vec3d(speedBarSize, 10.0, speedBarSize));
	_speedBarBackgroundPat->setAttitude(osg::Quat(osg::DegreesToRadians(270.0f), osg::Vec3(1.0f, 0.0f , 0.0f)));
	_speedPat->addChild(_speedBarBackgroundPat);

	_speedBarPat = new osg::PositionAttitudeTransform();
	_speedBarPat->addChild(_speedBarNode);
	_speedBarPat->setScale(osg::Vec3d(speedBarSize / 5.0f, speedBarSize / 6.0f, speedBarSize / 5.0f));
	_speedBarPat->setAttitude(osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3(0.0f, 1.0f , 0.0f)));
	_speedBarMatrixTrans->addChild(_speedBarPat);
	
	_speedPat->setPosition(osg::Vec3(speedBarSize * 2 + 20, speedBarSize * 2 + 20 , 0));
}

void HeadUpDisplay::initializeTimer()
{
    resetTimer();
    
	_timeNode = new osg::Geode();

	_timer = new osgText::Text();
	_timer->setFont(TIMER_FONT);
	
	int timerSize = viewer.getCamera()->getViewport()->height() / 25;
    _timer->setCharacterSize(timerSize);
    
    // place timer on top right
	_timer->setPosition(osg::Vec3(viewer.getCamera()->getViewport()->width() - timerSize * 6, 
	                                viewer.getCamera()->getViewport()->height() - timerSize - 20, 0));
	
    _timeNode->addDrawable(_timer);
	_hudPat->addChild(_timeNode);
}

void HeadUpDisplay::updateSpeedometer()
{
	float playerSpeed = Player::getInstance()->getPlayerState()->getSpeed();
	_speedBarMatrixTrans->setMatrix(osg::Matrix::rotate(osg::inDegrees(-(playerSpeed*270) -135), 0.0f, 0.0f, 1.0f));
 }

void HeadUpDisplay::updateTimer()
{
    time_t _timePassed;
    
    if(_isTiming) 
        _timePassed = getTime();
    else
        _timePassed = getFinalTime();

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

    _timer->setText(ss.str());

}

time_t HeadUpDisplay::getTime()
{
    struct timeb currentTime;

    // get current time
    ftime(&currentTime);
    
    // calculate offset from start time
    time_t _timePassed = (((currentTime.time - _startTime.time) * 1000) + (currentTime.millitm - _startTime.millitm)) / 10;
    
    return _timePassed;
}

void HeadUpDisplay::startTimer()
{
    _isTiming = true;
    resetTimer();
}

void HeadUpDisplay::stopTimer()
{
    _isTiming = false;
    _finalTime = getTime();
}

void HeadUpDisplay::resetTimer()
{
    ftime(&_startTime);
}

void HeadUpDisplayUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    osg::ref_ptr<HeadUpDisplay> hud = dynamic_cast<HeadUpDisplay *> (node->getUserData());

    hud->updateSpeedometer();    
    hud->updateTimer();
}