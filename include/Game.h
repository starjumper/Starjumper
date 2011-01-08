#pragma once

#include "RenderingInstance.h"

class Game : public RenderingInstance
{
private:
public:
    Game();
    
    virtual void configureViewer(osgViewer::Viewer *viewer);
};