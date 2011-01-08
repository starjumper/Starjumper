#pragma once

#include "RenderingInstance.h"

class Menu : public RenderingInstance
{
private:
public:
    Menu();
    
    void configureViewer(osgViewer::Viewer *viewer);
};