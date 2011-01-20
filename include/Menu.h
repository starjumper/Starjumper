#pragma once

#include <iostream>

#if defined (_WIN32)
    #include <functional>
#else
	#include <tr1/functional>
#endif

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/WindowManager>
#include <osgWidget/Box>

#include "types.h"
#include "RenderingInstance.h"
#include "MenuButton.h"

#define MENU_BACKGROUND_COLOR   1.0f, 1.0f, 1.0f, 0.0f

// masks for the window manager
const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;

class Menu : public RenderingInstance
{
private:
	osgWidget::WindowManager *_windowManager;
	osgWidget::Box *_buttonList;

public:
	Menu(osgViewer::Viewer *viewer);
	void addButton(const char* label, std::tr1::function<void ()> callback);
};