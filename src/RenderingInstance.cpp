#include "RenderingInstance.h"

RenderingInstance::RenderingInstance(osgViewer::Viewer *viewer): _viewer(viewer)
{
    _rootNode = new osg::Group();
}

osg::Group *RenderingInstance::getRootNode()
{
    return _rootNode;
}

osgViewer::Viewer *RenderingInstance::getViewer()
{
    return _viewer;
}