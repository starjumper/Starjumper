#pragma once

#include <osgGA/GUIEventHandler>

#include "types.h"
#include "Player.h"

class LevelKeyboardHandler : public osgGA::GUIEventHandler
{    
public:
    LevelKeyboardHandler();

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);
};