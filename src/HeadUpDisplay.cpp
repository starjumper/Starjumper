#include "HeadUpDisplay.h"

// _camera -> _node
//         -> _pat ->



HeadUpDisplay::HeadUpDisplay(Player *player) : 
    _player(player)
{
    _node = new osg::Geode;
    _node->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    
    _node->setUserData(this);
    _node->setUpdateCallback(new HeadUpDisplayUpdateCallback);
    
	initializeCamera();
	initializeSpeedBar();
    initializeTimer();
}

void HeadUpDisplay::initializeCamera()
{
	_camera = new osg::Camera();
	_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT));
	_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	_camera->setViewMatrix(osg::Matrix::identity());
	_camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	_camera->setRenderOrder(osg::Camera::POST_RENDER);
}

osg::Camera *HeadUpDisplay::getCamera()
{
    return _camera;
}

void HeadUpDisplay::initializeSpeedBar()
{
	_pat = new osg::PositionAttitudeTransform;
    _camera->addChild(_pat);

    _speedBar = new osg::ShapeDrawable(new osg::Box(SPEEDBAR_POSITION, SPEEDBAR_WIDTH, 250, 5));
	_node->addDrawable(_speedBar);
	//_pat->addChild(_node);
	_pat->setPosition(osg::Vec3f(300, 150, 0));
}

void HeadUpDisplay::initializeTimer()
{
    _startTime = clock();
    
	_timer = new osgText::Text();
	_timer->setFont(TIMER_FONT);
	_timer->setPosition(TIMER_POSITION);
	
    _node->addDrawable(_timer);
}


void HeadUpDisplay::updateSpeedBar()
{
	float playerSpeed = _player->getPlayerState()->getSpeed();
    _speedBar->setColor(osg::Vec4(1.0, playerSpeed, 0.0, 0.5));
	_pat->setAttitude(osg::Quat(osg::DegreesToRadians(-80+(playerSpeed*150.0)), 1.0, 0.0, 0.0));
}

void HeadUpDisplay::updateTimer()
{
	std::stringstream ss;
	ss << (clock() - _startTime) / 1000;
	std::string timeString = ss.str();
	
    if(timeString.size() > 2)
    {
        timeString.insert(timeString.size() - 2, ":", 1);
    }
    
    _timer->setText(timeString);
}

void HeadUpDisplayUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    osg::ref_ptr<HeadUpDisplay> hud = dynamic_cast<HeadUpDisplay *> (node->getUserData());

    hud->updateSpeedBar();
    hud->updateTimer();
}