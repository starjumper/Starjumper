#pragma once

#include <tr1/functional>

#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>

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