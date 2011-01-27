#include "PlayerController.h"

PlayerController::PlayerController(Player *player) :
    _player(player)
{

}

void PlayerController::setPlayer(Player *player)
{
    _player = player;
}

bool PlayerController::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    bool keyState;

    // check if key is pressed or released
    switch(ea.getEventType())
	{
	    case osgGA::GUIEventAdapter::KEYDOWN:
			keyState = K_PRESSED;
			break;
		case osgGA::GUIEventAdapter::KEYUP:
			keyState = K_RELEASED;
			break;
		default:
			return false;
	}

    // find out which key was pressed
	switch(ea.getKey())
	{
		case K_LEFT:
            _player->moveLeft(keyState);
			break;
		case K_RIGHT:
			_player->moveRight(keyState);
			break;
		case K_UP:
			_player->accelerate(keyState);
			break;
		case K_DOWN:
			_player->decelerate(keyState);
			break;
		case K_JUMP:
			_player->jump(keyState);
			break;
		case K_EXIT:
            exit(0);     // TODO: replace this by something useful
            break;
		default:
			return false;
	}

	return true;

}

void PlayerController::accept(osgGA::GUIEventHandlerVisitor &v)
{
    v.visit(*this);
}