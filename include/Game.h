#pragma once

#include "RenderingInstance.h"
#include "types.h"

class Game : public RenderingInstance
{
private:
    
public:
    Game(osgViewer::Viewer *viewer);

    virtual void configureViewer(osgViewer::Viewer *viewer);
};