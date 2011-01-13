#include "Menu.h"

Menu::Menu() :
    RenderingInstance()
{
    
}


osgWidget::Label* createLabel(const std::string& l, unsigned int size=13) {
	osgWidget::Label* label = new osgWidget::Label("", "");
	
	    label->setFont("fonts/times.ttf");
    	label->setFontSize(size);
	    label->setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	    label->setLabel(l);
		label->setPadding(3.0f);
		label->setColor(0.0f, 0.0f, 1.0f, 0.5f);
		label->setAlignHorizontal(osgWidget::Widget::HA_LEFT);
	
	    return label;
}

void Menu::configureViewer(osgViewer::Viewer *viewer)
{
    osgWidget::WindowManager* wm = new osgWidget::WindowManager(
        viewer,
       	viewer->getCamera()->getViewport()->width(),
       	viewer->getCamera()->getViewport()->height(),
        MASK_2D,
        // osgWidget::WindowManager::WM_USE_RENDERBINS |
        osgWidget::WindowManager::WM_PICK_DEBUG
    );
    
    osgWidget::Box*   vbox   = new osgWidget::Box("vbox", osgWidget::Box::VERTICAL);

    // Setup the labels for the vertical box.
    osgWidget::Label* label1 = createLabel("Start game", 60);
    osgWidget::Label* label2 = createLabel("Settings", 60);
    osgWidget::Label* label3 = createLabel("Highscore", 60);
    osgWidget::Label* label4 = createLabel("Quit", 60);

    vbox->addWidget(label1);
    vbox->addWidget(label2);
    vbox->addWidget(label3);
    vbox->addWidget(label4);
    vbox->attachMoveCallback();
    vbox->attachScaleCallback();

    vbox->resize();

    vbox->setVisibilityMode(osgWidget::Window::VM_ENTIRE);
//    vbox->setVisibleArea(50, 50, 500, 200);
    vbox->setAnchorVertical(osgWidget::Window::VA_CENTER);
    vbox->setAnchorHorizontal(osgWidget::Window::HA_CENTER);

	viewer->getCamera()->setClearColor(osg::Vec4( 0., 0., 0., 1. )); 

    // Test our label-in-window copy construction...
    wm->addChild(vbox);

    osg::Camera* camera = wm->createParentOrthoCamera();

    getRootNode()->addChild(camera);
    wm->resizeAllWindows();

//    osgWidget::createExample(*viewer, wm);
}

void Menu::handleUserInput(Key key, KeyState keyState)
{
    
}