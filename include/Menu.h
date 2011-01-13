#pragma once

#include <iostream>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/Util>
#include <osgWidget/WindowManager>
#include <osgWidget/Box>
#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Geode>


#include "RenderingInstance.h"
#include "types.h"

const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;

struct ColorLabel: public osgWidget::Label {
	ColorLabel(const char* label): osgWidget::Label("", "") 
	{
		setFont("fonts/times.ttf");
		setFontSize(14);
		setFontColor(0.0f, 0.0f, 0.0f, 1.0f);
//		setColor(0.5f, 0.5f, 0.5f, 1.0f);
//		addHeight(18.0f);
//		setCanFill(true);
		setLabel(label);
		setPadding(10.0f);
		addSize(21.0f, 22.0f);
		setColor(1.0f, 0.5f, 0.0f, 0.0f);
//		setEventMask(osgWidget::EVENT_MOUSE_PUSH | osgWidget::EVENT_MASK_MOUSE_MOVE);
	}
/*
	bool mousePush(double, double, osgWidget::WindowManager*) 
	{
		return true;
	}

	bool mouseEnter(double, double, osgWidget::WindowManager*) 
	{
		setColor(0.6f, 0.6f, 0.6f, 1.0f);
        return true;
    }

    bool mouseLeave(double, double, osgWidget::WindowManager*) 
	{
        setColor(0.3f, 0.3f, 0.3f, 1.0f);
        return true;
    }*/
};

class ColorLabelMenu: public ColorLabel {
    osg::ref_ptr<osgWidget::Window> _window;

public:
    ColorLabelMenu(const char* label):
    ColorLabel(label) {
        _window = new osgWidget::Box(
            std::string("Menu_") + label,
            osgWidget::Box::VERTICAL,
            true
        );

        _window->addWidget(new ColorLabel("Open Some Stuff"));
        _window->addWidget(new ColorLabel("Do It Now"));
        _window->addWidget(new ColorLabel("Hello, How Are U?"));
        _window->addWidget(new ColorLabel("Hmmm..."));
        _window->addWidget(new ColorLabel("Option 5"));

        _window->resize();

        setColor(0.8f, 0.8f, 0.8f, 0.8f);
    }

    void managed(osgWidget::WindowManager* wm) {
        osgWidget::Label::managed(wm);

        wm->addChild(_window.get());

        _window->hide();
    }

    void positioned() {
        osgWidget::Label::positioned();

        _window->setOrigin(getX(), getHeight());
        _window->resize(getWidth());
    }

    bool mousePush(double, double, osgWidget::WindowManager*) {
        if(!_window->isVisible()) _window->show();

        else _window->hide();

        return true;
    }

    bool mouseLeave(double, double, osgWidget::WindowManager*) {
        if(!_window->isVisible()) setColor(0.8f, 0.8f, 0.8f, 0.8f);

        return true;
    }
};

class Menu : public RenderingInstance
{
private:
    
public:
	Menu();
	
	virtual void configureViewer(osgViewer::Viewer *viewer);
    virtual void handleUserInput(Key key, KeyState keyState);
};