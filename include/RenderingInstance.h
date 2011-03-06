#pragma once

#include <osg/Group>
#include <osgViewer/Viewer>

#include "types.h"

class RenderingInstance
{
private:
    osg::ref_ptr<osg::Group> _rootNode;
	osgViewer::Viewer *_viewer;

public:
    RenderingInstance(osgViewer::Viewer *viewer);
    osg::Group *getRootNode();
    osgViewer::Viewer *getViewer();
    
    void clearSceneData();
    
    // methods to implement preparation before and cleanup
    // after switching RenderingInstances
    virtual void prepare(osgViewer::Viewer *viewer) = 0;
    virtual void cleanup(osgViewer::Viewer *viewer) = 0;
};