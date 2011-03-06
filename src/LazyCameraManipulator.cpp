#include "LazyCameraManipulator.h"

#include <iostream>

LazyCameraManipulator::LazyCameraManipulator():
	NodeTrackerManipulator() 
{   
    _oldNodePosition = osg::Vec3(0, 0, 0);
    _oldCameraPosition = osg::Vec3(0, 0, 0);
    _newCameraPosition = osg::Vec3(0, 0, 0);
    
    _directionOfMovementX = 0;
    _durationOfMovementX = 0;
    
    _firstRun = true;
    
	setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER);    	  
}

LazyCameraManipulator::~LazyCameraManipulator() 
{ 
    
}

bool LazyCameraManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us) 
{     
    if(ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
    {
        osg::Vec3d nodePosition;
        osg::Quat nodeRotation;
    
        computeNodeCenterAndRotation(nodePosition, nodeRotation);

        float positionDiffX = nodePosition.x() - _oldNodePosition.x();
        int newDirectionX = positionDiffX > 0 ? 1 : positionDiffX < 0 ? -1 : 0;

        if(fabs(_oldCameraPosition.x() - nodePosition.x()) < 0.0001 || _firstRun)
        {
            _newCameraPosition.x() = nodePosition.x();
            _oldCameraPosition.x() = nodePosition.x();
            _firstRun = false;
            return true;
        }

        if(newDirectionX == _directionOfMovementX)
        {
            _durationOfMovementX += 1;
            _newCameraPosition.x() = _oldCameraPosition.x() + (nodePosition.x() - _oldCameraPosition.x()) * (_durationOfMovementX / MAX_FRAME_DELAY);
        }
        else
        {
            if(newDirectionX != 0)
                _durationOfMovementX = 0;
            _directionOfMovementX = newDirectionX;
        }

        _oldNodePosition = nodePosition;
        _oldCameraPosition = _newCameraPosition;
     }

	return true;
}

osg::Matrixd LazyCameraManipulator::getInverseMatrix() const
{
    osg::Vec3d nodeCenter;
    osg::Quat nodeRotation;
    
    osg::Matrixd localToWorld, worldToLocal;
    computeNodeLocalToWorld(localToWorld);
    computeNodeWorldToLocal(worldToLocal);

    osg::Vec3d currentNodePosition = osg::Vec3d(_trackNodePath.back()->getBound().center()) * localToWorld;
    
    computeNodeCenterAndRotation(nodeCenter,nodeRotation);
                
    return osg::Matrixd::translate(-_newCameraPosition.x(), -nodeCenter[1], -nodeCenter[2]) * osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0,0.0,-_distance);
}

void LazyCameraManipulator::resetCamera()
{
    _firstRun = true;
}