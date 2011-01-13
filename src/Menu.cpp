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
}

void Menu::configureViewer(osgViewer::Viewer *viewer)
{
    _buttonList->attachMoveCallback();
    _buttonList->attachScaleCallback();
    _buttonList->resize();

    _windowManager->addChild(_buttonList);

    osg::Camera* camera = _windowManager->createParentOrthoCamera();

  	_buttonList->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
    _buttonList->resizePercent(100.0f);

    getRootNode()->addChild(camera);

    viewer->addEventHandler(new osgWidget::MouseHandler(_windowManager));

    _windowManager->resizeAllWindows();
}

void Menu::addButton(const char* label, std::tr1::function<void ()> callback)
{
	MenuButton *button = new MenuButton(label, callback);
	_buttonList->addWidget(button);
}
