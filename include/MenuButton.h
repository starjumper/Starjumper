#pragma once

#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>

class MenuButton: public osgWidget::Label 
{
private:
	void (*_callback)();
	
public:
	MenuButton(const char* label, void (*callback)());

	bool mousePush(double, double, osgWidget::WindowManager *);
	bool mouseEnter(double, double, osgWidget::WindowManager *);
	bool mouseLeave(double, double, osgWidget::WindowManager *);
};