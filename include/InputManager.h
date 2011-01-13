#pragma once

#include <osgGA/GUIEventHandler>

#include "types.h"
#include "GameManager.h"

class GameManager;

class InputManager : public osgGA::GUIEventHandler
{
private:
    GameManager *_manager;
    
public:  
    InputManager(GameManager *manager);
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);
};