#pragma once

#include <osgGA/GUIEventHandler>

#include "types.h"
#include "LevelMenu.h"

class MenuKeyboardHandler : public osgGA::GUIEventHandler
{
private:
    LevelMenu *_levelMenu;
    
public:
    MenuKeyboardHandler(LevelMenu *levelMenu);

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);
};