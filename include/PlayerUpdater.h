#pragma once

#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/PositionAttitudeTransform>

#include "Player.h"

class PlayerUpdater : public osg::NodeCallback 
{
private:
    Player *_player;
    
public:
    PlayerUpdater(Player *player);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};