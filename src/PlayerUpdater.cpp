#include "PlayerUpdater.h"

PlayerUpdater::PlayerUpdater(Player *player) :
    _player(player)
{
    
}

void PlayerUpdater::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    osg::PositionAttitudeTransform *playerPAT = dynamic_cast<osg::PositionAttitudeTransform *> (node);
  
    if(playerPAT)
    {
        osg::Vec3d position = playerPAT->getPosition();
        if(_player->_accelerate)
            position.y() += 1;
        if(_player->_moveLeft)
            position.x() -= 0.5;
        if(_player->_moveRight)
            position.x() += 0.5;
        if(_player->_decelerate)
            position.y() -= 1;
        
        playerPAT->setPosition(position);
    }
  
  traverse(node, nv); 
}