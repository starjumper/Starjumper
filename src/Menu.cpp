#include "Menu.h"

Menu::Menu(osgViewer::Viewer *viewer) :
    RenderingInstance(viewer)
{
    _windowManager = new osgWidget::WindowManager(
        viewer,
       	viewer->getCamera()->getViewport()->width(),
       	viewer->getCamera()->getViewport()->height(),
        MASK_2D
    );  

	_buttonList = new osgWidget::Box("menu", osgWidget::Box::VERTICAL);
	
	// position the menu in the window
	_buttonList->setAnchorVertical(osgWidget::Window::VA_CENTER);
    _buttonList->setAnchorHorizontal(osgWidget::Window::HA_CENTER);

  	_buttonList->getBackground()->setColor(MENU_BACKGROUND_COLOR);

    _buttonList->attachMoveCallback();
    _buttonList->attachScaleCallback();
    _buttonList->resize();

    _windowManager->addChild(_buttonList);
    _windowManager->resizeAllWindows();

	// create a camera which is orthogonal to the window for having a 2D view
    osg::Camera *camera = _windowManager->createParentOrthoCamera();
    getRootNode()->addChild(camera);
    
    getViewer()->addEventHandler(new osgWidget::MouseHandler(_windowManager));

	initializeBackground();
}

void Menu::initializeBackground()
{

	// set up animationpath
    osg::Vec3 center(0.0f,13.0f,5.0f);
    float radius = 4.5f;
	float looptime = 10.0f;


	osg::AnimationPath* animationPath = new osg::AnimationPath;
    animationPath->setLoopMode(osg::AnimationPath::LOOP);
    
    int numSamples = 40;
    float yaw = 0.0f;
    float yaw_delta = 2.0f*osg::PI/((float)numSamples-1.0f);
    float roll = osg::inDegrees(30.0f);
    
    double time=0.0f;
    double time_delta = looptime/(double)numSamples;
    for(int i=0;i<numSamples;++i)
    {
        osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
        osg::Quat rotation(osg::Quat(roll*time*2,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));
        
        animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

        yaw += yaw_delta;
        time += time_delta;
    }





    osg::Group* model = new osg::Group;
 
    osg::Node* ship = osgDB::readNodeFile(PLAYER_MODEL);
    if (ship)
    {
        const osg::BoundingSphere& bs = ship->getBound();

        float size = radius/bs.radius()*0.3f;
        osg::MatrixTransform* positioned = new osg::MatrixTransform;
        positioned->setDataVariance(osg::Object::STATIC);
        positioned->setMatrix(osg::Matrix::translate(-bs.center())*
                              osg::Matrix::scale(size,size,size)*
                              osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,0.0f,2.0f));
    
        positioned->addChild(ship);
    
        osg::MatrixTransform* xform = new osg::MatrixTransform;
        xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0f,2.0));
        xform->addChild(positioned);

        model->addChild(xform);
    }


	getRootNode()->addChild(model);
}

void Menu::addButton(const char* label, std::tr1::function<void ()> callback)
{
	MenuButton *button = new MenuButton(label, callback);
	
	_buttonList->addWidget(button);
}

void Menu::prepare(osgViewer::Viewer *viewer)
{
    
}

void Menu::cleanup(osgViewer::Viewer *viewer)
{
    
}