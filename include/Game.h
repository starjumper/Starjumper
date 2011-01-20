#pragma once

#include <osg/Group>

#include "RenderingInstance.h"
#include "LazyCameraManipulator.h"
#include "Level.h"
#include "Player.h"

class Game : public RenderingInstance
{
private:
    
    Level *_level;
    Player *_player;
    LazyCameraManipulator *_cameraManipulator;
    
public:
    Game(osgViewer::Viewer *viewer);

    virtual void configureViewer(osgViewer::Viewer *viewer);
};