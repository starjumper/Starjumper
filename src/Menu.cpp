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
        MASK_2D
    );
    
    osgWidget::Box*   vbox   = new osgWidget::Box("vbox", osgWidget::Box::VERTICAL);

    // Setup the labels for the vertical box.
    MenuButton *startButton = new MenuButton("Start game", NULL);
    MenuButton *settingsButton = new MenuButton("Settings", NULL);
    MenuButton *highscoreButton = new MenuButton("Highscore", NULL);
    MenuButton *quitButton = new MenuButton("Quit", NULL);

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

    viewer->addEventHandler(new osgWidget::MouseHandler(wm));

    wm->resizeAllWindows();
}

void Menu::handleUserInput(Key key, KeyState keyState)
{
    
}