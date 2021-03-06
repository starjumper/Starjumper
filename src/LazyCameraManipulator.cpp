#include "LazyCameraManipulator.h"
#include "Player.h"

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
    _fadeOut = false;
    
	setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER);    	  
}

LazyCameraManipulator::~LazyCameraManipulator() 
{ 
    
}

bool LazyCameraManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us) 
{
    return false;
}

void LazyCameraManipulator::calculateNextCameraPosition()
{ 
    // retrieve player position and direction of movement
    const Player *player = dynamic_cast<const Player *>(getTrackNode());
    const osg::Vec3d nodePosition = player->getPosition();
    const PlayerState *playerState = player->getPlayerState();

    const int newDirectionX = playerState->getDirectionX();

    // if this is the first run, follow node directly
    if(_firstRun)
    {
        _newCameraPosition.x() = nodePosition.x();
        _oldCameraPosition.x() = nodePosition.x();
        _oldNodePosition = nodePosition;
        _durationOfMovementX = 0;
        _firstRun = false;
        return;
    }

    // +++ step 1 +++
    // check if direction of movement has changed
    bool directionChanged = false;
    if(newDirectionX != _directionOfMovementX)
    {
        if(newDirectionX != 0)
        {
            _durationOfMovementX = 0;
            directionChanged = true;
        }
        _directionOfMovementX = newDirectionX;
    }
            
    float stepper = (_numSimulationSubSteps > 0) ? ((nodePosition.x() - _oldNodePosition.x()) / _numSimulationSubSteps) : 0.0f;
    
    for(int i=0; i<_numSimulationSubSteps; i++, _oldNodePosition.x() += stepper, _durationOfMovementX++)
    {
        if(_fadeOut)
            _distance += 10.0f * _numSimulationSubSteps;
        
        if(!directionChanged && fabs(_oldCameraPosition.x() - _oldNodePosition.x()) < 0.001 && _durationOfMovementX > 20)
        {
            // +++ step 2 +++
            // if direction has not changed, check if we were already following the node
            _newCameraPosition.x() = nodePosition.x();
        }
        else
        {
            // +++ step 3 +++
            // in any other case continue (or begin) approaching the node
            _newCameraPosition.x() = _oldCameraPosition.x() + (_oldNodePosition.x() - _oldCameraPosition.x()) * (_durationOfMovementX / MAX_FRAME_DELAY);
        }
    
        _oldCameraPosition = _newCameraPosition;
    }
    
    _oldNodePosition = nodePosition;
}

osg::Matrixd LazyCameraManipulator::getInverseMatrix() const
{
    const Player *player = dynamic_cast<const Player *>(getTrackNode());
    const osg::Vec3d nodePosition = player->getPosition();
        
    osg::Vec3d nc;
    osg::Quat nodeRotation;
    computeNodeCenterAndRotation(nc, nodeRotation);
        
    return osg::Matrixd::translate(-_newCameraPosition.x(), -nodePosition[1], -nodePosition[2]) * osg::Matrixd::rotate(nodeRotation.inverse())*osg::Matrixd::rotate(_rotation.inverse())*osg::Matrixd::translate(0.0,0.0,-_distance);
}

void LazyCameraManipulator::resetCamera()
{
    _firstRun = true;
    _fadeOut = false;
}

void LazyCameraManipulator::fadeOut()
{
    _fadeOut = true;
}

