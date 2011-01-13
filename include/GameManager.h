#pragma once

#include <map>
#include <string>

#include <osgViewer/Viewer>

#include "types.h"
#include "InputManager.h"
#include "RenderingInstance.h"
#include "Menu.h"
#include "Game.h"

class InputManager;

class GameManager
{
private:
    osgViewer::Viewer _viewer;
    std::map<std::string, RenderingInstance *> _renderingInstances;
    RenderingInstance *_activeRenderingInstance;
    InputManager *_inputManager;
    
    void addRenderingInstance(std::string name, RenderingInstance *instance);
    void selectRenderingInstance(std::string name);
    
public:
    GameManager();
    void run();
    void handleUserInput(Key key, KeyState keyState);
};