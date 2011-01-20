#pragma once

#include <osgGA/NodeTrackerManipulator>

#define MAX_CAMERA_DISTANCE_TO_MODEL 2.0f
#define RESET_LAZY_CAMERA_INTERVAL   0.1f

class LazyCameraManipulator : public osgGA::NodeTrackerManipulator
{
    private:
    
    public:
        LazyCameraManipulator(); 
        
        osg::Matrixd getInverseMatrix() const;
        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

    protected:

        ~LazyCameraManipulator();
        
        float                   _oldNodeXPosition;
        float                   _newCameraXPosition;
};
