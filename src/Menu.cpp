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

  	_buttonList->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);

    _buttonList->attachMoveCallback();
    _buttonList->attachScaleCallback();
    _buttonList->resize();

    _windowManager->addChild(_buttonList);
    _windowManager->resizeAllWindows();

	// create a camera which is orthogonal to the window for having a 2D view
    osg::Camera *camera = _windowManager->createParentOrthoCamera();
    getRootNode()->addChild(camera);
}

void Menu::configureViewer(osgViewer::Viewer *viewer)
{
    viewer->addEventHandler(new osgWidget::MouseHandler(_windowManager));
}

void Menu::addButton(const char* label, std::tr1::function<void ()> callback)
{
	MenuButton *button = new MenuButton(label, callback);
	
	_buttonList->addWidget(button);
}
