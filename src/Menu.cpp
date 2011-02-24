#include "Menu.h"

// test
#include "Player.h"

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





	// menu background ship
	osg::ref_ptr<osg::Node> playerModel = osgDB::readNodeFile(PLAYER_MODEL);
	if(!playerModel)
    {
        throw std::runtime_error("Unable to load player model file!");
    }
	
	osg::PositionAttitudeTransform *backgroundModel = new osg::PositionAttitudeTransform;
	backgroundModel->addChild(playerModel);
	backgroundModel->setPosition(osg::Vec3(0.0,5.0,5.0));
	getRootNode()->addChild(backgroundModel);
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