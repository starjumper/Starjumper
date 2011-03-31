#pragma once

#include <osgGA/GUIEventHandler>
#include <Sound.h>

#include "types.h"
#include "Player.h"
#include "PlayerState.h"

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