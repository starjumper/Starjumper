#pragma once

#include "RenderingInstance.h"
#include "types.h"

class Menu : public RenderingInstance
{
private:
    
public:
    Menu();
    virtual void configureViewer(osgViewer::Viewer *viewer);
    virtual void handleUserInput(Key key, KeyState keyState);
};