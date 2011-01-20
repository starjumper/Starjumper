#include "MenuButton.h"

MenuButton::MenuButton(const char* label, std::tr1::function<void ()> callback) : 
	osgWidget::Label("", ""), 
	_callback(callback)
{	
	// styling
	setFont("fonts/times.ttf");
	setFontSize(MENU_BUTTON_FONT_SIZE);
    setFontColor(MENU_BUTTON_FONT_COLOR);
    setLabel(label);
	setPadding(MENU_BUTTON_PADDING);
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
    setFontColor(MENU_BUTTON_FONT_COLOR_HOVER);

    return true;
}

bool MenuButton::mouseLeave(double, double, osgWidget::WindowManager*) 
{
	// reset style
	setFontColor(MENU_BUTTON_FONT_COLOR);
			
    return true;
}