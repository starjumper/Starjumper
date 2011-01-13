#pragma once

#include <osg/Group>
#include <osgViewer/Viewer>

#include "types.h"

class RenderingInstance
{
private:
    osg::ref_ptr<osg::Group> _rootNode;
public:
    RenderingInstance();
    osg::Group *getRootNode();
    
    // configure global viewer according to the RenderingInstance's needs
    virtual void configureViewer(osgViewer::Viewer *viewer) = 0;
    
    // react upon user input
    virtual void handleUserInput(Key key, KeyState keyState) = 0;
};