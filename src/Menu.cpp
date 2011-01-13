#include "Menu.h"

Menu::Menu() :
    RenderingInstance()
{
    
}

void Menu::configureViewer(osgViewer::Viewer *viewer)
{
    osgWidget::WindowManager* wm = new osgWidget::WindowManager(
        viewer,
       	viewer->getCamera()->getViewport()->width(),
       	viewer->getCamera()->getViewport()->height(),
        MASK_2D,
        osgWidget::WindowManager::WM_USE_RENDERBINS |
        osgWidget::WindowManager::WM_PICK_DEBUG
    );
    
    osgWidget::Box*   vbox   = new osgWidget::Box("vbox", osgWidget::Box::VERTICAL);

    // Setup the labels for the vertical box.
    MenuButton *startButton = new MenuButton("Start game");
    MenuButton *settingsButton = new MenuButton("Settings");
    MenuButton *highscoreButton = new MenuButton("Highscore");
    MenuButton *quitButton = new MenuButton("Quit");

    vbox->addWidget(quitButton);
    vbox->addWidget(highscoreButton);
    vbox->addWidget(settingsButton);
    vbox->addWidget(startButton);

    vbox->attachMoveCallback();
    vbox->attachScaleCallback();
    vbox->resize();

    vbox->setAnchorVertical(osgWidget::Window::VA_CENTER);
    vbox->setAnchorHorizontal(osgWidget::Window::HA_CENTER);

    wm->addChild(vbox);

    osg::Camera* camera = wm->createParentOrthoCamera();

  	vbox->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
    vbox->resizePercent(100.0f);

    getRootNode()->addChild(camera);
    wm->resizeAllWindows();
}

void Menu::handleUserInput(Key key, KeyState keyState)
{
    
}