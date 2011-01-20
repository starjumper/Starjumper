#pragma once

#include <osg/Group>

#include "RenderingInstance.h"
#include "Level.h"

class Game : public RenderingInstance
{
private:
    
    Level *_level;
    
public:
    Game(osgViewer::Viewer *viewer);

    virtual void configureViewer(osgViewer::Viewer *viewer);
};