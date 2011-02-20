#pragma once

#include <osgGA/GUIEventHandler>

#include "Player.h"
#include "PlayerState.h"

#define K_LEFT  osgGA::GUIEventAdapter::KEY_Left
#define K_RIGHT osgGA::GUIEventAdapter::KEY_Right
#define K_UP    osgGA::GUIEventAdapter::KEY_Up
#define K_DOWN  osgGA::GUIEventAdapter::KEY_Down
#define K_JUMP  osgGA::GUIEventAdapter::KEY_Space
#define K_EXIT  osgGA::GUIEventAdapter::KEY_Escape

#define K_PRESSED  true
#define K_RELEASED false

class GameKeyboardHandler : public osgGA::GUIEventHandler
{
private:
    Player *_player;
    
public:
    GameKeyboardHandler(Player *player);
    void setPlayer(Player *player);
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    virtual void accept(osgGA::GUIEventHandlerVisitor &v);
};