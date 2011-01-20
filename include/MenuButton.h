#pragma once

#if defined (_WIN32)
    #include <functional>
#else
	#include <tr1/functional>
#endif

#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>

#define MENU_BUTTON_FONT_COLOR          0.8f, 0.85f, 0.9f, 0.8f
#define MENU_BUTTON_FONT_COLOR_HOVER    0.8f, 0.85f, 0.9f, 1.0f
#define MENU_BUTTON_FONT_SIZE           60
#define MENU_BUTTON_PADDING             3.0f

class MenuButton: public osgWidget::Label 
{
private:
	std::tr1::function<void ()> _callback;
	
public:
	MenuButton(const char* label, std::tr1::function<void ()> callback);

	bool mousePush(double, double, osgWidget::WindowManager *);
	bool mouseEnter(double, double, osgWidget::WindowManager *);
	bool mouseLeave(double, double, osgWidget::WindowManager *);
};