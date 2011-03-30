#pragma once

#include <stdexcept>

#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#define PLAYER_MODEL            "resources/models/player.osg"
#define PLAYER_SCALE            osg::Vec3(0.2, 0.2, 0.2)
#define PLAYER_ATTITUDE         osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3(0.0,0.0,1.0))
#define PLAYER_HOME_POSITION    osg::Vec3(0.0, 10.0, 5.0)

class Player : public osg::PositionAttitudeTransform
{
private:
    static Player *_instance;
    
    Player();
    
    void loadPlayerModel();
    void resetPosition();
        
public:
    static Player *getInstance();
};
