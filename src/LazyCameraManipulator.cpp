#include "LazyCameraManipulator.h"

LazyCameraManipulator::LazyCameraManipulator():
	NodeTrackerManipulator() 
{
    _newCameraXPosition = 0.0f;
    
	setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER);    	  
}

LazyCameraManipulator::~LazyCameraManipulator() 
{ 
    
}

bool LazyCameraManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us) 
{     
    if(ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
    {
        osg::Vec3d nodeCenter;
        osg::Quat nodeRotation;
    
        computeNodeCenterAndRotation(nodeCenter,nodeRotation);

        float diff = _oldNodeXPosition - nodeCenter[0];

        if(fabs(diff) < 0.00001) 
        {
            if(_newCameraXPosition - nodeCenter[0] > 0.00001)
            {
                _newCameraXPosition -= RESET_LAZY_CAMERA_INTERVAL;
            }
            
            if(_newCameraXPosition - nodeCenter[0] < 0.00001)
            {
                _newCameraXPosition += RESET_LAZY_CAMERA_INTERVAL;
            }
        }
        else 
        {
            _newCameraXPosition -= diff / 1.5f;
            
            if(_newCameraXPosition > nodeCenter[0] + MAX_CAMERA_DISTANCE_TO_MODEL)
            {
                _newCameraXPosition = nodeCenter[0] + MAX_CAMERA_DISTANCE_TO_MODEL;
            }
            else if(_newCameraXPosition < nodeCenter[0] - MAX_CAMERA_DISTANCE_TO_MODEL)
            {
                _newCameraXPosition = nodeCenter[0] - MAX_CAMERA_DISTANCE_TO_MODEL;
            }
        }

     
        _oldNodeXPosition = nodeCenter[0];
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
                
    return osg::Matrixd::translate(-_newCameraXPosition, -nodeCenter[1], -nodeCenter[2]) * osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0,0.0,-_distance);
}
