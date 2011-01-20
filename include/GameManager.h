#pragma once

#include <map>
#include <string>

#if defined (_WIN32)
    #include <functional>
#else
	#include <tr1/functional>
#endif

#include <osgViewer/Viewer>

#include "types.h"
#include "RenderingInstance.h"
#include "Menu.h"
#include "Game.h"

class GameManager
{
private:
    osgViewer::Viewer _viewer;
    std::map<std::string, RenderingInstance *> _renderingInstances;
    RenderingInstance *_activeRenderingInstance;
    
    void addRenderingInstance(std::string name, RenderingInstance *instance);
    void selectRenderingInstance(std::string name);
    
	void buildMenus();

public:
    GameManager();

    void run();
	void quit();
    void startGame();
};