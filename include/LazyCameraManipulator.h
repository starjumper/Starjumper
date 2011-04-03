#pragma once

#include <osgGA/NodeTrackerManipulator>

#define MAX_FRAME_DELAY 100.0

class LazyCameraManipulator : public osgGA::NodeTrackerManipulator
{
    private:
        osg::Vec3 _oldNodePosition;
        osg::Vec3 _oldCameraPosition;
        osg::Vec3 _newCameraPosition;
        int _numSimulationSubSteps;
        
        int _directionOfMovementX;
        size_t _durationOfMovementX;
        
        bool _firstRun;
        bool _fadeOut;
        
    public:
        LazyCameraManipulator(); 
        
        osg::Matrixd getInverseMatrix() const;
        bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
        void calculateNextCameraPosition();
        
        void resetCamera();
        void fadeOut();
        
        void setNumSimulationSubSteps(int numSimulationSubSteps) { _numSimulationSubSteps = numSimulationSubSteps; };

    protected:
        ~LazyCameraManipulator();
};
