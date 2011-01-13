#pragma once

#include "RenderingInstance.h"
#include "types.h"

class Game : public RenderingInstance
{
private:
    
public:
    Game();
    virtual void configureViewer(osgViewer::Viewer *viewer);
    virtual void handleUserInput(Key key, KeyState keyState);
};