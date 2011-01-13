#include "MenuButton.h"

MenuButton::MenuButton(const char* label, void (*callback)()) : 
	osgWidget::Label("", ""), 
	_callback(callback)
{	
	// styling
	setFont("fonts/times.ttf");
	setFontSize(60);
    setFontColor(0.8f, 0.85f, 0.9f, 0.8f);
    setLabel(label);
	setPadding(3.0f);
	setAlignHorizontal(osgWidget::Widget::HA_LEFT);
    
	// activate event handling for mouse clicks and moves
	setEventMask(osgWidget::EVENT_MOUSE_PUSH | osgWidget::EVENT_MASK_MOUSE_MOVE);
}

bool MenuButton::mousePush(double, double, osgWidget::WindowManager*) 
{
	_callback();
	
	return true;
}

bool MenuButton::mouseEnter(double, double, osgWidget::WindowManager*) 
{
	// hover style
    setFontColor(0.8f, 0.85f, 0.9f, 1.0f);

    return true;
}

bool MenuButton::mouseLeave(double, double, osgWidget::WindowManager*) 
{
	// reset style
	setFontColor(0.8f, 0.85f, 0.9f, 0.8f);
			
    return true;
}