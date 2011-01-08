#include "RenderingInstance.h"

RenderingInstance::RenderingInstance()
{
    _rootNode = new osg::Group();
}

osg::Group *RenderingInstance::getRootNode()
{
    return _rootNode;
}